#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

import iroha
import commons

admin = commons.user('admin@test')
alice = commons.user('alice@test')
bob = commons.user('bob@test')


def genesis_tx():
    test_permissions = iroha.StringVector()
    test_permissions.append('can_grant_can_set_my_account_detail')
    test_permissions.append('can_set_detail')
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(admin['id']) \
        .addPeer('0.0.0.0:50541', admin['key'].publicKey()) \
        .createRole('admin_role', commons.all_permissions()) \
        .createRole('test_role', test_permissions) \
        .createDomain('test', 'test_role') \
        .createAccount('admin', 'test', admin['key'].publicKey()) \
        .createAccount('alice', 'test', alice['key'].publicKey()) \
        .createAccount('bob', 'test', bob['key'].publicKey()) \
        .appendRole(admin['id'], 'admin_role') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(admin['key']).finish()


def grant_permission():
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(alice['id']) \
        .grantPermission(bob['id'], 'can_set_my_account_detail') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(alice['key']).finish()


def set_detail_tx():
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(bob['id']) \
        .setAccountDetail(alice['id'], 'bobs', 'call') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(bob['key']).finish()


print(admin['key'].privateKey().hex())
print(genesis_tx().hex())
print(grant_permission().hex())
print(set_detail_tx().hex())
