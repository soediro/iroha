#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

import iroha
import commons

admin = commons.user('admin@first')
alice = commons.user('alice@second')

def genesis_tx():
    test_permissions = iroha.StringVector()
    test_permissions.append('can_get_all_signatories')
    tx = iroha.ModelTransactionBuilder() \
        .createdTime(commons.now()) \
        .creatorAccountId(admin['id']) \
        .addPeer('0.0.0.0:50541', admin['key'].publicKey()) \
        .createRole('admin_role', commons.all_permissions()) \
        .createRole('test_role', test_permissions) \
        .createDomain('first', 'admin_role') \
        .createDomain('second', 'test_role') \
        .createAccount('admin', 'first', admin['key'].publicKey()) \
        .createAccount('alice', 'second', alice['key'].publicKey()) \
        .build()
    return iroha.ModelProtoTransaction(tx) \
        .signAndAddSignature(admin['key']).finish()


def signatories_query():
    tx = iroha.ModelQueryBuilder() \
        .createdTime(commons.now()) \
        .queryCounter(1) \
        .creatorAccountId(alice['id']) \
        .getSignatories(admin['id']) \
        .build()
    return iroha.ModelProtoQuery(tx) \
        .signAndAddSignature(alice['key']).finish()


print(admin['key'].privateKey().hex())
print(genesis_tx().hex())
print(signatories_query().hex())
