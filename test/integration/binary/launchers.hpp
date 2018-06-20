/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef IROHA_LAUNCHERS_HPP
#define IROHA_LAUNCHERS_HPP

#include <boost/process.hpp>
#include <chrono>
#include <string>
#include "backend/protobuf/queries/proto_query.hpp"
#include "backend/protobuf/transaction.hpp"
#include "cryptography/keypair.hpp"

namespace binary_test {

  class Launcher {
   public:
    virtual std::string launchCommand(const std::string &test_case) = 0;

    void operator()(const std::string &example);

    bool initialized(const unsigned &transactions_expected = 0,
                     const unsigned &queries_expected = 0);

    std::shared_ptr<shared_model::crypto::Keypair> admin_key;
    std::vector<shared_model::proto::Transaction> transactions;
    std::vector<shared_model::proto::Query> queries;

   protected:
    void readBinaries(boost::process::ipstream &stream);
    void checkAsserts(const unsigned &transactions_expected,
                      const unsigned &queries_expected);
  };

  class PythonLauncher : public Launcher {
   public:
    virtual std::string launchCommand(const std::string &example);
  };

  class JavaLauncher : public Launcher {
   public:
    virtual std::string launchCommand(const std::string &example);
  };

}  // namespace binary_test

#endif  // IROHA_LAUNCHERS_HPP
