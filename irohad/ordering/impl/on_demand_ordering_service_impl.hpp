/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_ON_DEMAND_ORDERING_SERVICE_IMPL_HPP
#define IROHA_ON_DEMAND_ORDERING_SERVICE_IMPL_HPP

#include "ordering/on_demand_ordering_service.hpp"

#include <tbb/concurrent_queue.h>
#include <boost/thread.hpp>

#include <mutex>
#include <queue>
#include <unordered_map>

namespace iroha {
  namespace ordering {
    class OnDemandOrderingServiceImpl : public OnDemandOrderingService {
     public:
      // --------------------- | OnDemandOrderingService | ---------------------

      void onCollaborationOutcome(RoundOutput outcome,
                                  transport::RoundType round) override;

      // ----------------------- | OdOsNotification | --------------------------

      void onTransactions(
          std::vector<std::shared_ptr<shared_model::interface::Transaction>>
              transactions) override;

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
       */
      void insertLast(RoundOutput outcome,
                      const transport::RoundType &last_round);

      /**
       * Removes last elements
       * Method removes the oldest commit or chain of the oldest rejects
       */
      void erase();

      /**
       * @return packed proposal from current round queue
       */
      ProposalType emitProposal();

      /**
       * Queue which holds all round in linear sequential
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
          current_proposal_ = std::make_pair(
              std::make_pair(2, 1), tbb::concurrent_queue<TransactionType>());

      /**
       * Lock for onCollaborationOutcome critical section
       */
      boost::shared_mutex lock_;
    };
  }  // namespace ordering
}  // namespace iroha
#endif  // IROHA_ON_DEMAND_ORDERING_SERVICE_IMPL_HPP
