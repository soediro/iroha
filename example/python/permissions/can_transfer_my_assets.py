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
    test_permissions.append('can_grant_can_transfer_my_assets')
    test_permissions.append('can_receive')
    test_permissions.append('can_transfer')
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
        .createAsset('coin', 'test', 2) \
        .addAssetQuantity(admin['id'], 'coin#test', '100.00') \
        .transferAsset(admin['id'], alice['id'], 'coin#test', 'init top up', '90.00') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(admin['key']).finish()


def grant_permission_tx():
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(alice['id']) \
        .grantPermission(bob['id'], 'can_transfer_my_assets') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(alice['key']).finish()


def transfer_asset_tx():
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(bob['id']) \
        .transferAsset(alice['id'], admin['id'], 'coin#test', 'transfer from alice to admin done by bob', '60.00') \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(bob['key']).finish()


print(admin['key'].privateKey().hex())
print(genesis_tx().hex())
print(grant_permission_tx().hex())
print(transfer_asset_tx().hex())
