/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_ON_DEMAND_ORDERING_SERVICE_IMPL_HPP
#define IROHA_ON_DEMAND_ORDERING_SERVICE_IMPL_HPP

#include "ordering/on_demand_ordering_service.hpp"

#include <tbb/concurrent_queue.h>
#include <boost/thread.hpp>

#include <queue>
#include <unordered_map>

namespace iroha {
  namespace ordering {
    class OnDemandOrderingServiceImpl : public OnDemandOrderingService {
     public:
      OnDemandOrderingServiceImpl(
          size_t transaction_limit,
          size_t number_of_proposals = 3,
          const transport::RoundType &initial_round = std::make_pair(2, 1));

      // --------------------- | OnDemandOrderingService |
      // ---------------------

      void onCollaborationOutcome(RoundOutput outcome,
                                  transport::RoundType round) override;

      // ----------------------- | OdOsNotification | --------------------------

      void onTransactions(const std::vector<
                          std::shared_ptr<shared_model::interface::Transaction>>
                              &transactions) override;

      boost::optional<std::shared_ptr<shared_model::interface::Proposal>>
      onRequestProposal(transport::RoundType round) override;

     private:
      /**
       * Type of stored transactions
       */
      using TransactionType =
          std::shared_ptr<shared_model::interface::Transaction>;

      /**
       * Type of stored proposals
       */
      using ProposalType = std::shared_ptr<shared_model::interface::Proposal>;

      /**
       * Packs new proposal and creates new round
       * Note: method is not thread-safe
       */
      void packNextProposal(RoundOutput outcome,
                            const transport::RoundType &last_round);

      /**
       * Removes last elements if it is required
       * Method removes the oldest commit or chain of the oldest rejects
       * Note: method is not thread-safe
       */
      void tryErase();

      /**
       * @return packed proposal from current round queue
       * Note: method is not thread-safe
       */
      ProposalType emitProposal();

      /**
       * Max number of transaction in one proposal
       */
      size_t transaction_limit_;

      /**
       * Max number of available proposals in one OS`1234567890'
       */
      size_t number_of_proposals_;

      /**
       * Queue which holds all round in linear order
       */
      std::queue<transport::RoundType> round_queue_;

      /**
       * Map of available proposals
       */
      std::unordered_map<transport::RoundType,
                         ProposalType,
                         transport::RoundTypeHasher>
          proposal_map_;

      /**
       * Proposal for current round
       */
      std::pair<transport::RoundType, tbb::concurrent_queue<TransactionType>>
          current_proposal_;

      /**
       * Lock for onCollaborationOutcome critical section
       */
      boost::shared_mutex lock_;
    };
  }  // namespace ordering
}  // namespace iroha
#endif  // IROHA_ON_DEMAND_ORDERING_SERVICE_IMPL_HPP
