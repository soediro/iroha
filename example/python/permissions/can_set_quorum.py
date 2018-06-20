#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

import iroha
import commons

admin = commons.user('admin@test')
alice = commons.user('alice@test')


def genesis_tx():
    test_permissions = iroha.StringVector()
    test_permissions.append('can_set_quorum')
    extra_key = iroha.ModelCrypto().generateKeypair()
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(admin['id']) \
        .addPeer('0.0.0.0:50541', admin['key'].publicKey()) \
        .createRole('admin_role', commons.all_permissions()) \
        .createRole('test_role', test_permissions) \
        .createDomain('test', 'test_role') \
        .createAccount('admin', 'test', admin['key'].publicKey()) \
        .createAccount('alice', 'test', alice['key'].publicKey()) \
        .appendRole(admin['id'], 'admin_role') \
        .addSignatory(alice['id'], extra_key.publicKey()) \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(admin['key']).finish()


def set_quorum_tx():
    # Quourum cannot be greater than amount of keys linked to an account
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(alice['id']) \
        .setAccountQuorum(alice['id'], 2) \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(alice['key']).finish()


print(admin['key'].privateKey().hex())
print(genesis_tx().hex())
print(set_quorum_tx().hex())
