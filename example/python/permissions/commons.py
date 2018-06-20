#
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
#

import iroha
from time import time


def now():
    return int(time() * 1000)


def all_permissions():
    raw_permissions = [
        "can_append_role",
        "can_create_role",
        "can_detach_role",
        "can_add_asset_qty",
        "can_subtract_asset_qty",
        "can_add_peer",
        "can_add_signatory",
        "can_remove_signatory",
        "can_set_quorum",
        "can_create_account",
        "can_set_detail",
        "can_create_asset",
        "can_transfer",
        "can_receive",
        "can_create_domain",
        "can_read_assets",
        "can_get_roles",
        "can_get_my_account",
        "can_get_all_accounts",
        "can_get_domain_accounts",
        "can_get_my_signatories",
        "can_get_all_signatories",
        "can_get_domain_signatories",
        "can_get_my_acc_ast",
        "can_get_all_acc_ast",
        "can_get_domain_acc_ast",
        "can_get_my_acc_detail",
        "can_get_all_acc_detail",
        "can_get_domain_acc_detail",
        "can_get_my_acc_txs",
        "can_get_all_acc_txs",
        "can_get_domain_acc_txs",
        "can_get_my_acc_ast_txs",
        "can_get_all_acc_ast_txs",
        "can_get_domain_acc_ast_txs",
        "can_get_my_txs",
        "can_get_all_txs",
        "can_grant_can_set_my_quorum",
        "can_grant_can_add_my_signatory",
        "can_grant_can_remove_my_signatory",
        "can_grant_can_transfer_my_assets",
        "can_grant_can_set_my_account_detail"
        ]
    permissions = iroha.StringVector()
    for permission in raw_permissions:
        permissions.append(permission)
    return permissions


def user(user_id):
    return {
        'id': user_id,
        'key': iroha.ModelCrypto().generateKeypair()
    }

