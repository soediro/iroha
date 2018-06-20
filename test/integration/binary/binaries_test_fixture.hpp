/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_BINARIES_TEST_FIXTURE_HPP
#define IROHA_BINARIES_TEST_FIXTURE_HPP

#include <gtest/gtest.h>

#include <vector>
#include "builders/protobuf/block.hpp"
#include "framework/integration_framework/integration_test_framework.hpp"
#include "framework/specified_visitor.hpp"
#include "launchers.hpp"

using namespace boost::process;
using namespace binary_test;
using namespace integration_framework;
using namespace shared_model;
using namespace shared_model::interface;

namespace shared_model {

  namespace proto {
    class Query;
  }

  namespace interface {

    class Block;
    class AccountDetailResponse;
    class AccountAssetResponse;
    class AccountResponse;
    class AssetResponse;
    class RolePermissionsResponse;
    class RolesResponse;
    class SignatoriesResponse;
    class TransactionsResponse;

  }  // namespace interface

}  // namespace shared_model

namespace query_validation {

  namespace internal {

    class Void {};

  }  // namespace internal

  using QueryIterator = std::vector<proto::Query>::iterator;

  template <typename ResponseType>
  inline void checkQueryResponse(
      const shared_model::proto::QueryResponse &response) {
    ASSERT_NO_THROW(boost::apply_visitor(
        framework::SpecifiedVisitor<ResponseType>(), response.get()));
  }

  template <typename Head, typename... Tail>
  inline void _validateQueries(QueryIterator it,
                               QueryIterator end,
                               IntegrationTestFramework &itf) {
    if (it != end) {
      itf.sendQuery(*it, checkQueryResponse<Head>);
      _validateQueries<Tail...>(++it, end, itf);
    }
  }

  template <>
  inline void _validateQueries<internal::Void>(QueryIterator it,
                                     QueryIterator end,
                                     IntegrationTestFramework &itf){};

  template <typename... Responses>
  inline void validateQueries(QueryIterator it,
                              QueryIterator end,
                              IntegrationTestFramework &itf) {
    _validateQueries<Responses..., internal::Void>(it, end, itf);
  }

}  // namespace query_validation

template <typename T>
class BinaryTestFixture : public ::testing::Test {
 public:
  T launcher;

  shared_model::proto::Block makeGenesis(
      const shared_model::proto::Transaction &genesis_tx,
      const shared_model::crypto::Keypair &keypair) {
    return shared_model::proto::BlockBuilder()
        .transactions(std::vector<shared_model::proto::Transaction>{genesis_tx})
        .height(1)
        .prevHash(shared_model::crypto::DefaultHashProvider::makeHash(
            shared_model::crypto::Blob("")))
        .createdTime(iroha::time::now())
        .build()
        .signAndAddSignature(keypair)
        .finish();
  }

  shared_model::proto::Block genesis() {
    return makeGenesis(launcher.transactions[0], *launcher.admin_key);
  }

  static void blockWithTransactionValidation(
      const std::shared_ptr<shared_model::interface::Block> &result) {
    ASSERT_EQ(result->transactions().size(), 1);
  }

  template <typename... QueryResponses>
  void doStupidTest(const unsigned &transactions_expected = 0,
                    const unsigned &queries_expected = 0) {
    if (launcher.initialized(transactions_expected, queries_expected)) {
      IntegrationTestFramework itf(1);

      itf.setInitialState(*launcher.admin_key, genesis());
      for (auto i = 1u; i < launcher.transactions.size(); ++i) {
        itf.sendTx(launcher.transactions[i])
            .checkBlock(blockWithTransactionValidation);
      }

      query_validation::validateQueries<QueryResponses...>(
          launcher.queries.begin(), launcher.queries.end(), itf);

      itf.done();
    }
  }

 protected:
  virtual void SetUp() {
    launcher(::testing::UnitTest::GetInstance()->current_test_info()->name());
  }
};

#endif  // IROHA_BINARIES_TEST_FIXTURE_HPP
