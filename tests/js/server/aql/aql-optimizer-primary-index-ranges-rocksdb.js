/*jshint globalstrict:false, strict:false, maxlen: 500 */
/*global assertTrue, assertEqual, assertNotEqual, AQL_EXPLAIN, AQL_EXECUTE */

////////////////////////////////////////////////////////////////////////////////
/// @brief tests for index usage
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2010-2012 triagens GmbH, Cologne, Germany
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
/// @author Jan Christoph Uhde
/// @author Copyright 2018, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

var jsunity = require("jsunity");
var db = require("@arangodb").db;

////////////////////////////////////////////////////////////////////////////////
/// @brief test suite
////////////////////////////////////////////////////////////////////////////////

function optimizerIndexesRangesTestSuite () {
  var c;

  return {
    setUp : function () {
      db._drop("UnitTestsCollection");
      c = db._create("UnitTestsCollection");

      for (var i = 0; i < 2000; ++i) {
        c.save({ _key: "test" + String(i).padStart(4,'0'), value1: (i % 100), value2: i });
      }
    },

    tearDown : function () {
      db._drop("UnitTestsCollection");
    },

////////////////////////////////////////////////////////////////////////////////
/// @brief test primary index usage
////////////////////////////////////////////////////////////////////////////////

    testPrimaryIndexRanges : function () {
      var queries = [ // queries[0] - query -- queries[1] - expected result
        // _key and _id mixed
        [
          "FOR i IN " + c.name() + " FILTER (i._key >= 'test1990' && i._id <= '" + c.name() + "/test2') RETURN i._key",
          [ "test1999", "test1998", "test1997", "test1996", "test1995", "test1994", "test1993", "test1992", "test1991", "test1990" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key > 'test1990' && i._id < '" + c.name() + "/test1999') RETURN i._key",
          [ "test1998", "test1997", "test1996", "test1995", "test1994", "test1993", "test1992", "test1991" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key > 'test1990' && i._id < '" + c.name() + "/test1999' && i._key < 'test1996') RETURN i._key",
          [ "test1995", "test1994", "test1993", "test1992", "test1991" ]
        ],

        // more than one condition
        [
          "FOR i IN " + c.name() + " FILTER (i._key >= 'test1990' && i._key <= 'test2') RETURN i._key",
          [ "test1999", "test1998", "test1997", "test1996", "test1995", "test1994", "test1993", "test1992", "test1991", "test1990" ]
        ],[
          "for i in " + c.name() + " filter (i._key > 'test1990' && i._key < 'test1999') return i._key",
          [ "test1998", "test1997", "test1996", "test1995", "test1994", "test1993", "test1992", "test1991" ]
        ],[
          "for i in " + c.name() + " filter (i._key > 'test1990' && i._key < 'test1999' && i._key < 'test1996') return i._key",
          [ "test1995", "test1994", "test1993", "test1992", "test1991" ]
        ],

        // tests for _key
        [
          "FOR i IN " + c.name() + " FILTER (i._key < 'test0002') RETURN i._key",
          [ "test0000", "test0001" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('test0002' > i._key) RETURN i._key",
          [ "test0000", "test0001" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key > 'test1997') RETURN i._key",
          [ "test1998", "test1999" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('test1997' < i._key) RETURN i._key",
          [ "test1998", "test1999" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key <= 'test0002') RETURN i._key",
          [ "test0000", "test0001", "test0002" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('test0002' >= i._key) RETURN i._key",
          [ "test0000", "test0001", "test0002" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key >= 'test1997') RETURN i._key",
          [ "test1997", "test1998", "test1999" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('test1997' <= i._key) RETURN i._key",
          [ "test1997", "test1998", "test1999" ]
        ],

        // tests for _id
        [
          "FOR i IN " + c.name() + " FILTER (i._id < '" + c.name() + "/test0002') RETURN i._key",
          [ "test0000", "test0001" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('" + c.name() + "/test0002' > i._id) RETURN i._key",
          [ "test0000", "test0001" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._id > '" + c.name() + "/test1997') RETURN i._key",
          [ "test1998", "test1999" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('" + c.name() + "/test1997' < i._id) RETURN i._key",
          [ "test1998", "test1999" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._id <= '" + c.name() + "/test0002') RETURN i._key",
          [ "test0000", "test0001", "test0002" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('" + c.name() + "/test0002' >= i._id) RETURN i._key",
          [ "test0000", "test0001", "test0002" ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._id >= '" + c.name() + "/test1997') RETURN i._key",
          [ "test1997", "test1998", "test1999" ]
        ],[
          "FOR i IN " + c.name() + " FILTER ('" + c.name() + "/test1997' <= i._id) RETURN i._key",
          [ "test1997", "test1998", "test1999" ]
        ],

        //// edge cases
        [
        // one element in range
          "FOR i IN " + c.name() + " FILTER ('test1997' <= i._key && 'test1997' >= i._key) RETURN i._key",
          [ "test1997" ]
        ]
      ]; //end of array

      queries.forEach(function(query) {
        var plan = AQL_EXPLAIN(query[0]).plan;
        var nodeTypes = plan.nodes.map(function(node) {
          return node.type;
        });

        //db._explain(query[0]);
        // ensure an index is used
        assertNotEqual(-1, nodeTypes.indexOf("IndexNode"), query);

        var results = AQL_EXECUTE(query[0]);

        assertEqual(query[1].length , results.json.length, query);
        var last = query[1][0];
        results.json.forEach(function(value) {
          assertNotEqual(-1, query[1].indexOf(value));
        });

        assertTrue(results.stats.scannedIndex > 0);
        assertEqual(0, results.stats.scannedFull);
      });
    },



    testPrimaryIndexRangesEdgeCases : function () {
      var queries = [ // queries[0] - query -- queries[1] - expected result
        [
          // upper is greater than lower
          "FOR i IN " + c.name() + " FILTER (i._key <= 'test1997' && i._key >= 'test1998') RETURN i._key",
          [ ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key == 'test1997' && i._key >= 'test1997') RETURN i._key",
          [ 'test1997' ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key == 'test1997' && i._key == 'test1998') RETURN i._key",
          [  ]
        ],[
          "FOR i IN " + c.name() + " FILTER (i._key < true) RETURN i._key",
          [  ]
        ]
      ]; //end of array

      queries.forEach(function(query) {
        var plan = AQL_EXPLAIN(query[0]).plan;
        var nodeTypes = plan.nodes.map(function(node) {
          return node.type;
        });

        var results = AQL_EXECUTE(query[0]);

        assertEqual(query[1].length , results.json.length, query);
        var last = query[1][0];
        results.json.forEach(function(value) {
          assertNotEqual(-1, query[1].indexOf(value));
        });

        assertEqual(0, results.stats.scannedFull);
      });
    }

///////////////////////////////////////////////////////////////////////////////

  };
}
jsunity.run(optimizerIndexesRangesTestSuite);

return jsunity.done();

