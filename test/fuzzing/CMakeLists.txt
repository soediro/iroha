# Copyright Soramitsu Co., Ltd. All Rights Reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=fuzzer,address")

add_executable(torii_fuzzing torii_fuzz.cpp)
target_link_libraries(torii_fuzzing
  rxcpp
  gtest
  gmock
  ametsuchi
  libs_common
  torii_service
  )
