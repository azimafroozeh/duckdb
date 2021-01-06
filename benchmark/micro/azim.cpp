#include "benchmark_runner.hpp"
#include "duckdb_benchmark_macro.hpp"

using namespace duckdb;
using namespace std;

//////////////
// INSERT //
//////////////
#define AZIM_BENCHMARK(CREATE_STATEMENT)                                                                               \
	void Load(DuckDBBenchmarkState *state) override {                                                                  \
		state->conn.Query(CREATE_STATEMENT);                                                                           \
		for (int32_t i = 0; i < 6400; i++) {                                                                          \
			state->conn.Query("INSERT INTO test VALUES (0)");                                   \
		}                                                                                                                    \
        state->conn.Query("CREATE VIEW v1 AS SELECT i FROM (SELECT first.i FROM (test AS first JOIN test AS second ON first.i = second.i) AS j) AS B"); \
		state->conn.Query("CREATE TABLE test2 AS SELECT i FROM (SELECT first.i FROM (test AS first JOIN test AS second ON first.i = second.i) AS j) AS B"); \
        state->conn.Query("PRAGMA enable_profiling;");\
	}                                                                                                                  \
	void RunBenchmark(DuckDBBenchmarkState *state) override {                                                          \
			state->conn.Query("SELECT (i + i + i + i + i + i + i + i + i +"                                         \
		                                    "i + i + i + i + i + i + i + i + i +"                                      \
		                                    "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i +"\
                                            "i + i + i + i + i + i + i + i + i)  FROM test2;");                \
	}                                                                                                                  \
	void Cleanup(DuckDBBenchmarkState *state) override {                                                               \
		state->conn.Query("DROP TABLE test");                                                                      \
		Load(state);                                                                                                   \
	}                                                                                                                  \
	string VerifyResult(QueryResult *result) override {                                                                \
		return string();                                                                                               \
	}                                                                                                                  \
	string BenchmarkInfo() override {                                                                                  \
		return "AZIM benchmark";                                                                                       \
	}

DUCKDB_BENCHMARK(azim, "[azim]")
AZIM_BENCHMARK("CREATE TABLE test(i INTEGER NOT NULL)")
FINISH_BENCHMARK(azim)

