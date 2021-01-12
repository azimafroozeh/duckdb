#include "duckdb.hpp"
#include "iostream"

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
            con.Query("PRAGMA enable_profiling;");
            if (arg == "1") {

                string query =
                    R"(SELECT       plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,
                                    i ))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))
                                    FROM (SELECT CAST(j % 2 as TINYINT) AS i FROM range(0, 300000000) tbl(j)) AS T)";

                auto result = con.Query(query);
            }
			else if(arg == "2")
			{
                string query =
                    R"(SELECT  (i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
                                i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i +
	                            i + i + i + i + i + i + i + i + i + i )

                                FROM (SELECT CAST(j % 2 as TINYINT) AS i FROM range(0, 300000000) tbl(j)) AS T)";

                auto result = con.Query(query);
			}
		}
	}



}

//"SELECT plus(i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ("
//"                                   i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i ,plus ( i , i ))))))))))))))))))))))))))"
//"  )))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))) FROM (SELECT CAST(j % 2 as TINYINT) AS i FROM range(0, 300000000) tbl(j)) AS T"
