/**
 * Copyright Soramitsu Co., Ltd. 2018 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ametsuchi/impl/postgres_ordering_service_persistent_state.hpp"
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include "common/types.hpp"

namespace iroha {
  namespace ametsuchi {

    expected::Result<std::shared_ptr<PostgresOrderingServicePersistentState>,
                     std::string>
    PostgresOrderingServicePersistentState::create(
        const std::string &postgres_options) {
      auto sql = std::make_unique<soci::session>(soci::postgresql, postgres_options);

      expected::Result<std::shared_ptr<PostgresOrderingServicePersistentState>,
                       std::string>
          storage;
      storage = expected::makeValue(
          std::make_shared<PostgresOrderingServicePersistentState>(std::move(sql)));
      return storage;
    }

    PostgresOrderingServicePersistentState::
        PostgresOrderingServicePersistentState(
            std::unique_ptr<soci::session> sql)
        : sql_(std::move(sql)),
          log_(logger::log("PostgresOrderingServicePersistentState")) {}

    bool PostgresOrderingServicePersistentState::initStorage() {
      *sql_ << "CREATE TABLE IF NOT EXISTS ordering_service_state (proposal_height bigserial)";
      *sql_ << "INSERT INTO ordering_service_state VALUES (2)";
      return true;
    }

    bool PostgresOrderingServicePersistentState::dropStorgage() {
      log_->info("Drop storage");
      *sql_ << "DROP TABLE IF EXISTS ordering_service_state";
      return true;
    }

    bool PostgresOrderingServicePersistentState::saveProposalHeight(
        size_t height) {
      log_->info("Save proposal_height in ordering_service_state "
                 + std::to_string(height));
      *sql_ << "DELETE FROM ordering_service_state";
      *sql_ << "INSERT INTO ordering_service_state VALUES (:height)", soci::use(height);
      return true;
    }

    boost::optional<size_t>
    PostgresOrderingServicePersistentState::loadProposalHeight() const {
      boost::optional<size_t> height;
      *sql_ << "SELECT * FROM ordering_service_state LIMIT 1", soci::into(height);

      if (not height) {
        log_->error(
            "There is no proposal_height in ordering_service_state. "
                "Use default value 2.");
        height = 2;
      }
      return height;
    }

    bool PostgresOrderingServicePersistentState::resetState() {
      return dropStorgage() & initStorage();
    }

  }  // namespace ametsuchi
}  // namespace iroha
