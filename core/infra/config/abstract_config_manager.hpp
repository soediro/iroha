/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef IROHA_CONFIG_H
#define IROHA_CONFIG_H

#include <fstream>  // ifstream, ofstream
#include <json.hpp>
#include <utils/expected.hpp>
#include <utils/logger.hpp>
#include "config_utils.hpp"

namespace config {

using json = nlohmann::json;

class AbstractConfigManager {
 private:
  std::string readConfigData(const std::string& pathToJSONFile,
                             const std::string& defaultValue) {
    std::ifstream ifs(pathToJSONFile);
    if (ifs.fail()) {
      return defaultValue;
    }

    std::istreambuf_iterator<char> it(ifs);
    return std::string(it, std::istreambuf_iterator<char>());
  }

  json openConfigData() {
    auto configFolderPath = config::get_iroha_home() + this->getConfigName();
    auto jsonStr = readConfigData(configFolderPath, "");

    if (jsonStr.empty()) {
      logger::warning("config") << "there is no config '" << getConfigName()
                                << "', we will use default values.";
    } else {
      logger::debug("config") << "load json is " << jsonStr;
      parseConfigDataFromString(std::move(jsonStr));
    }

    return _configData;
  }

 protected:
  template <typename T>
  T getParam(std::initializer_list<const std::string> params,
             const T& defaultValue) {
    auto tempConfigData = getConfigData();
    try {
      size_t i = 0;
      for (auto& param : params) {
        ++i;
        if (i == params.size()) {
          return tempConfigData.value(param, defaultValue);
        }
        tempConfigData = tempConfigData[param];
      }
      return tempConfigData;
    } catch (...) {
      return defaultValue;
    }
  }

  virtual VoidHandler parseConfigDataFromString(std::string&& jsonStr) {
    try {
      _configData = json::parse(std::move(jsonStr));
      return {};
    } catch (...) {
      return makeUnexpected(exception::config::ParseException(getConfigName()));
    }
  }

 public:
  virtual std::string getConfigName() = 0;

  json getConfigData() {
    if (_loaded) {
      // If defaultValue is used, _configData is empty, but _loaded = true. It's
      return this->_configData;
    } else {
      _loaded = true;
      return openConfigData();
    }
  }

 protected:
  bool _loaded = false;
  json _configData;
};
}  // namespace config

#endif  // IROHA_CONFIG_H
