//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parallel/thread_context.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/main/query_profiler.hpp"

namespace duckdb {
class ClientContext;

//! The ThreadContext holds thread-local info for parallel usage
class ThreadContext {
public:
	explicit ThreadContext(ClientContext &context, int id);

	//! The operator profiler for the individual thread context
	OperatorProfiler profiler;
	int id;
};

class ThreadContext1 : public ThreadContext{
public:
	ThreadContext1(ClientContext context, int i);
	static const int b = 0;
};

} // namespace duckdb
