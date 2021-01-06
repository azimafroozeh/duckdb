#include "duckdb.hpp"

using namespace duckdb;

int main() {
    DuckDB db(nullptr);
    Connection con(db);
    // 2.9 * 1024 * 1024 * 1024 = 3113851289.6
    con.Query("CREATE TABLE test(i TINYINT NOT NULL); INSERT INTO test SELECT (i / 3113851289) AS i FROM range(0, 3113851289) tbl(i);");
    con.Query("PRAGMA enable_profiling;");
    auto result = con.Query("SELECT (i + i)  FROM test");

}