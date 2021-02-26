#include "duckdb.hpp"

using namespace duckdb;

int main() {
	DuckDB db(nullptr);
	Connection con(db);

	/*
	 * CREATE TABLE integers(i INTEGER, j INTEGER);

# SELECT * from table function
query ITTTTT
SELECT * FROM pragma_table_info('integers');
	 */
	con.Query("CREATE TABLE integers(i INTEGER, j INTEGER);");
	con.Query("INSERT INTO integers VALUES (3,3)");
	con.Query("Select min(i+i) from integers");
	auto result = con.Query("SELECT * FROM pragma_last_profiling_output()");
	result->Print();
}
