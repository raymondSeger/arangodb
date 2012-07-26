////////////////////////////////////////////////////////////////////////////////
/// @brief batch job
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triAGENS GmbH, Cologne, Germany
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
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Jan Steemann
/// @author Copyright 2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_REST_HANDLER_BATCH_JOB_H
#define TRIAGENS_REST_HANDLER_BATCH_JOB_H 1

#include "GeneralServer/GeneralServerJob.h"
#include "HttpServer/HttpServer.h"
#include "HttpServer/HttpHandler.h"
#include "Basics/Mutex.h"

// -----------------------------------------------------------------------------
// --SECTION--                                              forward declarations
// -----------------------------------------------------------------------------

namespace triagens {
  namespace rest {
    class BatchSubjob;

// -----------------------------------------------------------------------------
// --SECTION--                                            class GeneralServerJob
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GeneralServer
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief general server job
////////////////////////////////////////////////////////////////////////////////

    class BatchJob : public GeneralServerJob<HttpServer, HttpHandler> {
      private:
        BatchJob (BatchJob const&);
        BatchJob& operator= (BatchJob const&);

////////////////////////////////////////////////////////////////////////////////
/// @brief who accomplished the batch job?
////////////////////////////////////////////////////////////////////////////////

        enum Accomplisher {
          NOONE  = 0,
          TASK   = 1,
          DIRECT = 2,
          ASYNC  = 3
        };

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GeneralServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief constructs a new server job
////////////////////////////////////////////////////////////////////////////////

        BatchJob (HttpServer* server, HttpHandler* handler);

////////////////////////////////////////////////////////////////////////////////
/// @brief destructs a server job
////////////////////////////////////////////////////////////////////////////////

        ~BatchJob ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                       Job methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GeneralServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief directly execute job
////////////////////////////////////////////////////////////////////////////////

        void executeDirectHandler (HttpHandler*);

////////////////////////////////////////////////////////////////////////////////
/// @brief sub-job is done
////////////////////////////////////////////////////////////////////////////////

        void jobDone (BatchSubjob*);

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        status_e work ();

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        void cleanup ();

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        bool beginShutdown ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 protected methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GeneralServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief create a subjob
////////////////////////////////////////////////////////////////////////////////

        void createSubjob (HttpHandler*);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                               protected variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup GeneralServer
/// @{
////////////////////////////////////////////////////////////////////////////////

      protected:

////////////////////////////////////////////////////////////////////////////////
/// @brief who finalised the batch job?
////////////////////////////////////////////////////////////////////////////////

        Accomplisher _doneAccomplisher;

////////////////////////////////////////////////////////////////////////////////
/// @brief list of handlers
////////////////////////////////////////////////////////////////////////////////

        vector<HttpHandler*> _handlers;

////////////////////////////////////////////////////////////////////////////////
/// @brief list of sub-jobs
////////////////////////////////////////////////////////////////////////////////

        set<BatchSubjob*> _subjobs;

////////////////////////////////////////////////////////////////////////////////
/// @brief done lock
////////////////////////////////////////////////////////////////////////////////

        triagens::basics::Mutex _doneLock;

////////////////////////////////////////////////////////////////////////////////
/// @brief setup lock
////////////////////////////////////////////////////////////////////////////////

        triagens::basics::Mutex _setupLock;

////////////////////////////////////////////////////////////////////////////////
/// @brief number of completed jobs
////////////////////////////////////////////////////////////////////////////////

        size_t _jobsDone;

////////////////////////////////////////////////////////////////////////////////
/// @brief cleanup seen
////////////////////////////////////////////////////////////////////////////////

        bool _cleanup;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}\\)"
// End:
