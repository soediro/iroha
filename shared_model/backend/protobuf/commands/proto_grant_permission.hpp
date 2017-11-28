/**
 * Copyright Soramitsu Co., Ltd. 2017 All Rights Reserved.
 * http://soramitsu.co.jp
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef IROHA_PROTO_GRANT_PERMISSION_HPP
#define IROHA_PROTO_GRANT_PERMISSION_HPP

#include "interfaces/commands/grant_permission.hpp"

namespace shared_model {
  namespace proto {

    class GrantPermission final : public interface::GrantPermission {
     public:
      template <typename CommandType>
      explicit GrantPermission(CommandType &&command)
          : command_(std::forward<CommandType>(command)),
            grant_permission_([this] { return command_->grant_permission(); }) {
      }

      GrantPermission(const GrantPermission &o) : GrantPermission(*o.command_) {}

      GrantPermission(GrantPermission &&o) noexcept
          : GrantPermission(std::move(o.command_.variant())) {}

      const interface::types::AccountIdType &accountId() const override {
        return grant_permission_->account_id();
      }

      const interface::types::PermissionNameType &permissionName()
          const override {
        return iroha::protocol::GrantablePermission_Name(
            grant_permission_->permission());
      }

      ModelType *copy() const override {
        iroha::protocol::Command command;
        *command.mutable_grant_permission() = *grant_permission_;
        return new GrantPermission(std::move(command));
      }

     private:
      // proto
      detail::ReferenceHolder<iroha::protocol::Command> command_;

      template <typename Value>
      using Lazy = detail::LazyInitializer<Value>;
      const Lazy<const iroha::protocol::GrantPermission &> grant_permission_;
    };

  }  // namespace proto
}  // namespace shared_model

#endif  // IROHA_PROTO_GRANT_PERMISSION_HPP
