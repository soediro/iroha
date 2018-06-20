/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "launchers.hpp"

#include <gtest/gtest.h>
#include "common/byteutils.hpp"

#include "bindings/model_crypto.hpp"

using namespace boost::process;

namespace binary_test {

  constexpr auto cTimeToKill = std::chrono::minutes(15);

  void Launcher::operator()(const std::string &example) {
    ipstream pipe;
    const auto &command = launchCommand(example);
    if (command.empty()) {
      FAIL() << "Launcher provided empty command";
    }
    child c(command, std_out > pipe);
    auto terminated = c.wait_for(cTimeToKill);
    if (not terminated) {
      c.terminate();
      FAIL() << "Child process was terminated because execution time limit "
                "has been exceeded";
    }
    readBinaries(pipe);
  }

  void Launcher::readBinaries(ipstream &stream) {
    transactions.clear();
    queries.clear();
    std::string line;
    bool first_line(true);
    while (stream and std::getline(stream, line) and not line.empty()) {
      iroha::protocol::Transaction proto_tx;
      iroha::protocol::Query proto_query;
      if (auto byte_string = iroha::hexstringToBytestring(line)) {
        if (first_line) {
          first_line = false;
          admin_key = std::make_shared<shared_model::crypto::Keypair>(
              shared_model::bindings::ModelCrypto().fromPrivateKey(line));
        } else if (proto_tx.ParseFromString(*byte_string)) {
          transactions.emplace_back(std::move(proto_tx));
        } else if (proto_query.ParseFromString(*byte_string)) {
          queries.emplace_back(std::move(proto_query));
        }
      }
    }
  }

  bool Launcher::initialized(const unsigned &transactions_expected,
                             const unsigned &queries_expected) {
    checkAsserts(transactions_expected, queries_expected);
    return admin_key and (transactions.size() == transactions_expected)
        and (queries.size() == queries_expected);
  }

  void Launcher::checkAsserts(const unsigned &transactions_expected,
                              const unsigned &queries_expected) {
    ASSERT_TRUE(admin_key);
    ASSERT_EQ(transactions.size(), transactions_expected);
    ASSERT_EQ(queries.size(), queries_expected);
  }

  std::string PythonLauncher::launchCommand(const std::string &example) {
    std::stringstream s;
    s << "bash -c \"${PYTHON_INTERPRETER} "
         "${ROOT_DIR}/example/python/permissions/"
      << example << ".py\"";
    return s.str();
  }

  std::string JavaLauncher::launchCommand(const std::string &example) {
    return "";
    // tbd, igor-egorov, 2018-06-20, IR-1389
  }

}  // namespace binary_test
