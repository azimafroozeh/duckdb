#include "duckdb.hpp"

using namespace duckdb;

#define n
#define test_no

int main() {
    DuckDB db(nullptr);
    Connection con(db);

#ifdef test
    con.Query("CREATE TABLE test(i TINYINT NOT NULL); INSERT INTO test SELECT (i) AS i FROM range(0, 100) tbl(i);");
#else
    con.Query("CREATE TABLE test(i TINYINT NOT NULL); INSERT INTO test SELECT (i / 300000000) AS i FROM range(0, 300000000) tbl(i);");
#endif

	con.Query("PRAGMA show('table_name');");
    con.Query("PRAGMA enable_profiling;");
    auto result = con.Query("SELECT (i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i +"
	                        "           i + i + i + i + i + i + i + i + i + i + i ) FROM test");

}