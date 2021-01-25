//===----------------------------------------------------------------------===//
//
//                         DuckDB
//
// benchmark_runner.hpp
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include "benchmark_configuration.hpp"
#include "benchmark.hpp"
#include "duckdb/common/constants.hpp"
#include "duckdb/common/fstream.hpp"

namespace duckdb {
class DuckDB;

//! The benchmark runner class is responsible for running benchmarks
class BenchmarkRunner {
	BenchmarkRunner();

public:
	static constexpr const char *DUCKDB_BENCHMARK_DIRECTORY = "duckdb_benchmark_data";
	BenchmarkConfiguration configuration;

	static BenchmarkRunner &GetInstance() {
		static BenchmarkRunner instance;
		return instance;
	}

	//! Save the current database state, exporting it to a set of CSVs in the DUCKDB_BENCHMARK_DIRECTORY directory
	static void SaveDatabase(DuckDB &db, string name);
	//! Try to initialize the database from the DUCKDB_BENCHMARK_DIRECTORY
	static bool TryLoadDatabase(DuckDB &db, string name);

	//! Register a benchmark in the Benchmark Runner, this is done automatically
	//! as long as the proper macro's are used
	static void RegisterBenchmark(Benchmark *benchmark);

	void Log(string message);
	void LogLine(string message);
	void LogResult(string message);
	void LogOutput(string message);

	void RunBenchmark(Benchmark *benchmark);
	void RunPerf(Benchmark *benchmark);
	void RunBenchmarks();
	void RunPerfs();

	vector<Benchmark *> benchmarks;
	ofstream out_file;
	ofstream log_file;
};

} // namespace duckdb
