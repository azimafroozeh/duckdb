//===----------------------------------------------------------------------===//
//
//                         DuckDB
//
// benchmark.hpp
//
// Author: Mark Raasveldt
//
//===----------------------------------------------------------------------===//

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "benchmark_configuration.hpp"
#include "system.hpp"

namespace duckdb {

using std::string;
using std::unique_ptr;
using std::vector;

//! Base class for any state that has to be kept by a Benchmark
struct BenchmarkState {
	virtual ~BenchmarkState() {
	}
};

//! The base Benchmark class is a base class that is used to create and register
//! new benchmarks
class Benchmark {
	constexpr static size_t DEFAULT_NRUNS = 5;
	constexpr static size_t DEFAULT_TIMEOUT = 30;

	Benchmark(Benchmark &) = delete;

public:
	//! The name of the benchmark
	string name;
	//! The benchmark group this benchmark belongs to
	string group;

	Benchmark(bool register_benchmark, string name, string group);

	//! Initialize the benchmark state
	virtual unique_ptr<BenchmarkState> Initialize(BenchmarkConfiguration &config) {
		return nullptr;
	}
	//! Run the benchmark
	virtual void Run(BenchmarkState *state) = 0;
    //! Perf the benchmark
    virtual void Perf(BenchmarkState *state) {
        System::profile(name, [&]() {
          Run(state);
        });
	};
	//! Cleanup the benchmark, called after each Run
	virtual void Cleanup(BenchmarkState *state) = 0;
	//! Verify that the output of the benchmark was correct
	virtual string Verify(BenchmarkState *state) = 0;
	//! Finalize the benchmark runner
	virtual void Finalize() {
	}
	virtual string GetQuery() {
		return string();
	}
	virtual string DisplayName() {
		return name;
	}
	virtual string Group() {
		return group;
	}
	virtual string Subgroup() {
		return string();
	}
	//! Interrupt the benchmark because of a timeout
	virtual void Interrupt(BenchmarkState *state) = 0;
	//! Returns information about the benchmark
	virtual string BenchmarkInfo() = 0;

	string GetInfo() {
		return name + " - " + group + "\n" + BenchmarkInfo();
	}

	virtual string GetLogOutput(BenchmarkState *state) = 0;

	//! Whether or not Initialize() should be called once for every run or just
	//! once
	virtual bool RequireReinit() {
		return false;
	}
	//! The amount of runs to do for this benchmark
	virtual size_t NRuns() {
		return DEFAULT_NRUNS;
	}
	//! The timeout for this benchmark (in seconds)
	virtual size_t Timeout() {
		return DEFAULT_TIMEOUT;
	}
};

} // namespace duckdb
