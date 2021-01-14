#include "duckdb.hpp"
#include "iostream"

#include <duckdb/main/appender.hpp>

#include "System.h"
using namespace duckdb;


int main(int argc, char** argv) {
	DuckDB db(nullptr);
	Connection con(db);

	//#ifdef test
	//    con.Query("CREATE TABLE test(i TINYINT NOT NULL); INSERT INTO test SELECT (i) AS i FROM range(0, 100) tbl(i);");
	//#else
	//    con.Query("CREATE TABLE test(i TINYINT NOT NULL); INSERT INTO test SELECT (i / 300000000) AS i FROM range(0, 1024) tbl(i);");
	//	con.Query("TRANSACTION ");
	//#endif



    for (int i = 1; i < argc; ++i) {
		std::string arg = argv[i];
		if ((arg == "-h") || (arg == "--help")) {

		} else if ((arg == "-t") || (arg == "--test")) {
			arg = argv[++i];
            string query;
            if (arg == "3") {

                query =
                    R"(SELECT min ( plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    i )))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
                                    FROM (SELECT CAST(j % 2 as TINYINT) AS i FROM range(0, 300000000) tbl(j)) AS T)";

                auto result = con.Query(query);
            }
			else if(arg == "2")
			{
                query =
                    R"(SELECT  min(i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i )

                                FROM (SELECT CAST(j % 2 as TINYINT) AS i FROM range(0, 1000) tbl(j)) AS T)";

                auto result = con.Query(query);
			}
			else if(arg == "5") {

                con.Query("CREATE TABLE tbl AS SELECT (i % 2)::TINYINT i FROM range(300000000) tbl(i);");

				query = R"(select stats(i) from tbl limit 1)";

                auto result = con.Query(query);
                result->Print();

                con.Query("PRAGMA enable_profiling;");
                query =
                    R"(SELECT  min(i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i )
                                FROM tbl)";

                con.Query(query);

                query = R"(SELECT  stats(i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i )
                                FROM tbl limit 1)";

               result = con.Query(query);
			   result->Print();
			}
            else if(arg == "1") {
                // Load data
                System::profile("loading", [&]() {
                  con.Query("CREATE TABLE tbl AS SELECT (i % 2)::TINYINT i FROM range(300000000) tbl(i);");
                });

                // Run queries
                System::profile("queries", [&]() {
                  query =
                      R"(SELECT  min(i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i )
                                FROM tbl)";
                  con.Query(query);
                });



            }
			else if(arg == "4"){
                con.Query("CREATE TABLE tbl(i TINYINT NOT NULL)");
                Appender appender(con, "tbl");
                for(int i = 0; i < 300000000; ++i){
                    appender.AppendRow( i % 2);
                }
                appender.Close();
                query =
                    R"(SELECT  min(i + i + i + i + i + i + i + i + i + i +
                         i + i + i + i + i + i + i + i + i + i +
                            i + i + i + i + i + i + i + i + i + i +
                         i + i + i + i + i + i + i + i + i + i +
                            i + i + i + i + i + i + i + i + i + i +
                            i + i + i + i + i + i + i + i + i + i +
                            i + i + i + i + i + i + i + i + i + i +
                         i + i + i + i + i + i + i + i + i + i +
                         i + i + i + i + i + i + i + i + i + i +
                         i + i + i + i + i + i + i + i + i + i )
                            FROM tbl)";
                con.Query(query);
			}
		}
	}
}
