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
    test_permissions.append('can_append_role')
    test_permissions.append('can_add_peer')
    second_role = iroha.StringVector()
    second_role.append('can_add_peer')
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(admin['id']) \
        .addPeer('0.0.0.0:50541', admin['key'].publicKey()) \
        .createRole('admin_role', commons.all_permissions()) \
        .createRole('test_role', test_permissions) \
        .createRole('second_role', second_role) \
        .createDomain('test', 'test_role') \
        .createAccount('admin', 'test', admin['key'].publicKey()) \
        .createAccount('alice', 'test', alice['key'].publicKey()) \
        .createAccount('bob', 'test', bob['key'].publicKey()) \
        .appendRole(admin['id'], 'admin_role') \
        .appendRole(alice['id'], 'second_role') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(admin['key']).finish()


def append_role_tx():
    # Note that you can append only that role that has
    # lesser or the same set of permissions as transaction creator.
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(alice['id']) \
        .appendRole(bob['id'], 'second_role') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(alice['key']).finish()


print(admin['key'].privateKey().hex())
print(genesis_tx().hex())
print(append_role_tx().hex())
