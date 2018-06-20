/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#include "binaries_test_fixture.hpp"

using BinaryTestTypes = ::testing::Types<PythonLauncher, JavaLauncher>;

TYPED_TEST_CASE(BinaryTestFixture, BinaryTestTypes);

TYPED_TEST(BinaryTestFixture, can_create_account) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_set_detail) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_grant_can_set_my_account_detail) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_set_my_account_detail) {
  this->doStupidTest(3);
}

TYPED_TEST(BinaryTestFixture, can_create_asset) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_receive) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_transfer) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_grant_can_transfer_my_assets) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_transfer_my_assets) {
  this->doStupidTest(3);
}

TYPED_TEST(BinaryTestFixture, can_add_asset_qty) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_subtract_asset_qty) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_create_domain) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_add_peer) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_create_role) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_append_role) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_detach_role) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_add_signatory) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_grant_can_add_my_signatory) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_grant_can_remove_my_signatory) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_grant_can_set_my_quorum) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_add_my_signatory) {
  this->doStupidTest(3);
}

TYPED_TEST(BinaryTestFixture, can_remove_signatory) {
  this->doStupidTest(2);
}

TYPED_TEST(BinaryTestFixture, can_set_my_quorum) {
  this->doStupidTest(3);
}

TYPED_TEST(BinaryTestFixture, can_remove_my_signatory) {
  this->doStupidTest(3);
}

TYPED_TEST(BinaryTestFixture, can_set_quorum) {
  this->doStupidTest(2);
}

// -------------------------- Queries --------------------------

TYPED_TEST(BinaryTestFixture, can_get_all_acc_detail) {
  this->template doStupidTest<AccountDetailResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_all_accounts) {
  this->template doStupidTest<AccountResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_domain_acc_detail) {
  this->template doStupidTest<AccountDetailResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_domain_accounts) {
  this->template doStupidTest<AccountResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_my_acc_detail) {
  this->template doStupidTest<AccountDetailResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_my_account) {
  this->template doStupidTest<AccountResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_all_acc_ast) {
  this->template doStupidTest<AccountAssetResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_domain_acc_ast) {
  this->template doStupidTest<AccountAssetResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_my_acc_ast) {
  this->template doStupidTest<AccountAssetResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_all_acc_ast_txs) {
  this->template doStupidTest<TransactionsResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_domain_acc_ast_txs) {
  this->template doStupidTest<TransactionsResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_my_acc_ast_txs) {
  this->template doStupidTest<TransactionsResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_all_acc_txs) {
  this->template doStupidTest<TransactionsResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_domain_acc_txs) {
  this->template doStupidTest<TransactionsResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_my_acc_txs) {
  this->template doStupidTest<TransactionsResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_read_assets) {
  this->template doStupidTest<AssetResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_roles) {
  this->template doStupidTest<RolesResponse, RolePermissionsResponse>(1, 2);
}

TYPED_TEST(BinaryTestFixture, can_get_all_signatories) {
  this->template doStupidTest<SignatoriesResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_domain_signatories) {
  this->template doStupidTest<SignatoriesResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_my_signatories) {
  this->template doStupidTest<SignatoriesResponse>(1, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_all_txs) {
  this->template doStupidTest<TransactionsResponse>(3, 1);
}

TYPED_TEST(BinaryTestFixture, can_get_my_txs) {
  this->template doStupidTest<TransactionsResponse>(3, 1);
}
