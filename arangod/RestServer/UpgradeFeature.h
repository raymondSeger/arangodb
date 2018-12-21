////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2016 ArangoDB GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
////////////////////////////////////////////////////////////////////////////////

#ifndef APPLICATION_FEATURES_UPGRADE_FEATURE_H
#define APPLICATION_FEATURES_UPGRADE_FEATURE_H 1

#include "ApplicationFeatures/ApplicationFeature.h"
#include "VocBase/Methods/Upgrade.h"

namespace arangodb {

class UpgradeFeature final : public application_features::ApplicationFeature {
 public:
  UpgradeFeature(
    application_features::ApplicationServer& server,
    int* result,
    std::vector<std::string> const& nonServerFeatures
  );
  void addTask(methods::Upgrade::Task&& task);
  void collectOptions(std::shared_ptr<options::ProgramOptions>) override final;
  void validateOptions(std::shared_ptr<options::ProgramOptions>) override final;
  void prepare() override final;
  void start() override final;

 private:
  friend struct methods::Upgrade; // to allow access to '_tasks'

  bool _upgrade;
  bool _upgradeCheck;

  void upgradeDatabase();

  int* _result;
  std::vector<std::string> _nonServerFeatures;
  std::vector<methods::Upgrade::Task> _tasks;
};

}

#endif