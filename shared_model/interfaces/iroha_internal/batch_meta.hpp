/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
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

#ifndef IROHA_SHARED_MODEL_BATCH_META_HPP
#define IROHA_SHARED_MODEL_BATCH_META_HPP

#include "interfaces/base/model_primitive.hpp"
#include "interfaces/common_objects/types.hpp"

namespace shared_model {
  namespace interface {

    /**
     * Representation of fixed point number
     */
    class BatchMeta : public ModelPrimitive<BatchMeta> {
     public:
      virtual types::BatchType type() const = 0;

      std::string toString() const override {
        return detail::PrettyStringBuilder()
            .init("BatchMeta")
            .append("Type", type() == types::ATOMIC ? "ATOMIC" : "ORDERED")
            .appendAll(transactionHashes(),
                       [](auto &hash) { return hash.toString(); })
            .finalize();
      }
      /// type of hashes collection
      using TransactionHashesType = std::vector<interface::types::HashType>;

      /**
       * @return Hashes of transactions to fetch
       */
      virtual const TransactionHashesType &transactionHashes() const = 0;
      /**
       * Checks equality of objects inside
       * @param rhs - other wrapped value
       * @return true, if wrapped objects are same
       */
      bool operator==(const ModelType &rhs) const override {
        return type() == rhs.type();
      }
    };
  }  // namespace interface
}  // namespace shared_model
#endif  // IROHA_SHARED_MODEL_BATCH_META_HPP
