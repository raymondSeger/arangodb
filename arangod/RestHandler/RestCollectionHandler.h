////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2014-2017 ArangoDB GmbH, Cologne, Germany
/// Copyright 2004-2014 triAGENS GmbH, Cologne, Germany
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
/// @author Simon Grätzer
////////////////////////////////////////////////////////////////////////////////

#ifndef ARANGOD_REST_HANDLER_REST_COLLECTION_HANDLER_H
#define ARANGOD_REST_HANDLER_REST_COLLECTION_HANDLER_H 1

#include "RestHandler/RestVocbaseBaseHandler.h"

namespace arangodb {
class LogicalCollection;

class RestCollectionHandler : public arangodb::RestVocbaseBaseHandler {
 public:
  RestCollectionHandler(GeneralRequest*, GeneralResponse*);

 public:
  char const* name() const override final { return "RestCollectionHandler"; }
  bool isDirect() const override { return false; }
  RestStatus execute() override;

 private:
  void handleCommandGet();
  void handleCommandPost();
  void handleCommandPut();
  void handleCommandDelete();
  void collectionRepresentation(VPackBuilder& builder, std::string const& name,
                                bool showProperties, bool showFigures,
                                bool showCount, bool detailedCount);
  void collectionRepresentation(VPackBuilder& builder, LogicalCollection* coll,
                                bool showProperties, bool showFigures,
                                bool showCount, bool detailedCount);
};
}

#endif