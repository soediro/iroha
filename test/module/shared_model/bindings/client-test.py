# 
# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0
# 

from time import time
import unittest

import block_pb2 as blk
import commands_pb2 as cmd
import iroha

class ClientTest(unittest.TestCase):
  def setUp(self):
    self.keys = iroha.ModelCrypto().generateKeypair()

  def valid_add_peer_command(self):
    command = cmd.Command()
    command.add_peer.peer.address = "127.0.0.1:50500"
    command.add_peer.peer.peer_key = b'A' * 32
    return command

  def unsigned_tx(self):
    tx = blk.Transaction()
    tx.payload.reduced_payload.creator_account_id = "admin@test"
    tx.payload.reduced_payload.created_time = int(time() * 1000)
    tx.payload.reduced_payload.quorum = 1
    return tx

  def test_hash(self):
    tx = self.unsigned_tx()
    tx.payload.reduced_payload.commands.extend([self.valid_add_peer_command()])
    h = iroha.hash(tx.SerializeToString())
    self.assertEqual(len(h), 32)

  def test_sign(self):
    tx = self.unsigned_tx()
    tx.payload.reduced_payload.commands.extend([self.valid_add_peer_command()])
    self.assertEqual(len(tx.signatures), 0)
    tx_blob = iroha.sign(tx.SerializeToString(), self.keys)
    signed_tx = blk.Transaction()
    signed_tx.ParseFromString(bytearray(tx_blob))
    self.assertEqual(len(signed_tx.signatures), 1)

  def test_validate_without_cmd(self):
    tx = self.unsigned_tx()
    tx_blob = iroha.sign(tx.SerializeToString(), self.keys)
    print(iroha.validate(tx_blob))
    self.assertNotEqual(iroha.validate(tx_blob).find('Transaction should contain at least one command'), -1)

  def test_validate_unsigned_tx(self):
    tx = self.unsigned_tx()
    tx.payload.reduced_payload.commands.extend([self.valid_add_peer_command()])
    self.assertEqual(len(tx.signatures), 0)
    print(iroha.validate(tx.SerializeToString()))
    self.assertNotEqual(iroha.validate(tx.SerializeToString()).find('Signatures cannot be empty'), -1)

  def test_validate_correct_tx(self):
    tx = self.unsigned_tx()
    tx.payload.reduced_payload.commands.extend([self.valid_add_peer_command()])
    self.assertEqual(len(tx.signatures), 0)
    tx_blob = iroha.sign(tx.SerializeToString(), self.keys)
    self.assertEqual(iroha.validate(tx_blob), '')


if __name__ == '__main__':
  unittest.main()
