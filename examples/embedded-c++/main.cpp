#include "duckdb.hpp"

using namespace duckdb;

#define test_no

int main() {
    DuckDB db(nullptr);
    Connection con(db);

#ifdef test
    con.Query("CREATE TABLE test(i TINYINT NOT NULL); INSERT INTO test SELECT (i) AS i FROM range(0, 100) tbl(i);");
#else
    con.Query("CREATE TABLE test(i TINYINT NOT NULL); INSERT INTO test SELECT (i / 3221225472) AS i FROM range(0, 3221225472) tbl(i);");
#endif

	con.Query("PRAGMA show('table_name');");
    con.Query("PRAGMA enable_profiling;");
    auto result = con.Query("SELECT plus(i, i) FROM test");

}