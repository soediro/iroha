/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ordering/impl/on_demand_ordering_service_impl.hpp"

#include "builders/protobuf/proposal.hpp"
#include "datetime/time.hpp"
#include "interfaces/iroha_internal/proposal.hpp"
#include "interfaces/transaction.hpp"

using namespace iroha::ordering;

OnDemandOrderingServiceImpl::OnDemandOrderingServiceImpl(
    size_t transaction_limit,
    size_t number_of_proposals,
    const transport::RoundType &initial_round)
    : transaction_limit_(transaction_limit),
      number_of_proposals_(number_of_proposals),
      current_proposal_(std::make_pair(
          initial_round, tbb::concurrent_queue<TransactionType>())) {}

// -------------------------| OnDemandOrderingService |-------------------------

void OnDemandOrderingServiceImpl::onCollaborationOutcome(
    RoundOutput outcome, transport::RoundType round) {
  // exclusive write lock
  boost::upgrade_lock<boost::shared_mutex> upgrade_lock(lock_);
  boost::upgrade_to_unique_lock<boost::shared_mutex> guard(upgrade_lock);

  packNextProposal(outcome, round);
  tryErase();
}

// ----------------------------| OdOsNotification |-----------------------------

void OnDemandOrderingServiceImpl::onTransactions(
    const std::vector<std::shared_ptr<shared_model::interface::Transaction>>
        &transactions) {
  // read lock
  boost::shared_lock<boost::shared_mutex> guard(lock_);

  std::for_each(
      transactions.begin(), transactions.end(), [this](const auto &obj) {
        current_proposal_.second.push(obj);
      });
}

boost::optional<OnDemandOrderingServiceImpl::ProposalType>
OnDemandOrderingServiceImpl::onRequestProposal(transport::RoundType round) {
  // read lock
  boost::shared_lock<boost::shared_mutex> guard(lock_);
  auto proposal = proposal_map_.find(round);
  if (proposal != proposal_map_.end()) {
    return (*proposal).second;
  } else {
    return boost::none;
  }
}

// ---------------------------------| Private |---------------------------------

void OnDemandOrderingServiceImpl::packNextProposal(
    RoundOutput outcome, const transport::RoundType &last_round) {
  if (current_proposal_.second.empty()) {
    return;
  }

  round_queue_.push(current_proposal_.first);
  proposal_map_.insert(std::make_pair(current_proposal_.first, emitProposal()));

  auto current_round = current_proposal_.first;
  decltype(current_round) next_round;
  switch (outcome) {
    case RoundOutput::SUCCESSFUL:
      next_round = std::make_pair(current_round.first + 1, 1);
      break;
    case RoundOutput::REJECT:
      next_round =
          std::make_pair(current_round.first, current_round.second + 1);
      break;
  }

  current_proposal_.first = next_round;
  current_proposal_.second.clear();  // operator = of tbb::queue is closed
}

OnDemandOrderingServiceImpl::ProposalType
OnDemandOrderingServiceImpl::emitProposal() {
  auto mutable_proposal = shared_model::proto::ProposalBuilder()
                              .height(current_proposal_.first.first)
                              .createdTime(iroha::time::now());

  TransactionType current_tx;
  using ProtoTxType = shared_model::proto::Transaction;
  std::vector<TransactionType> collection;

  // outer method should guarantee availability of at least one transaction in
  // queue
  while (current_proposal_.second.try_pop(current_tx)
         and collection.size() <= transaction_limit_) {
    collection.emplace_back(current_tx);
  }
  auto proto_txes = collection | boost::adaptors::transformed([](auto &tx) {
                      return static_cast<const ProtoTxType &>(*tx);
                    });

  return std::make_shared<decltype(mutable_proposal.build())>(
      mutable_proposal.transactions(proto_txes).build());
}

void OnDemandOrderingServiceImpl::tryErase() {
  if (round_queue_.size() < number_of_proposals_) {
    return;
  }

  if (round_queue_.empty() and round_queue_.front().second == 1) {
    proposal_map_.erase(round_queue_.front());
    round_queue_.pop();
    return;
  } else {
    while (not round_queue_.empty() and round_queue_.front().second != 1) {
      proposal_map_.erase(round_queue_.front());
      round_queue_.pop();
    }
  }
}
