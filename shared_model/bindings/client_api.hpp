/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "cryptography/keypair.hpp"

namespace shared_model {
  namespace bindings {
    using Blob = std::vector<uint8_t>;

    /**
     * Validate protobuf object
     * @param blob to validate
     * @return string with errors, empty if none
     */
    std::string validate(const Blob &);

    /**
     * Signs protobuf object
     * @param blob to sign
     * @param key is keypair for signing
     * @return signed blob
     */
    Blob sign(const Blob &, const crypto::Keypair &);

    /**
     * Get the hash of given protobuf object
     * @param blob to calculate hash from
     * @return hash of the blob
     */
    Blob hash(const Blob &);
  }  // namespace bindings
}  // namespace shared_model
