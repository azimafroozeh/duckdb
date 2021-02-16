//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/execution/operator/projection/physical_projection.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/execution/physical_operator.hpp"

namespace duckdb {

class PhysicalProjection : public PhysicalOperator {
public:
	PhysicalProjection(vector<LogicalType> types, vector<unique_ptr<Expression>> select_list)
	    : PhysicalOperator(PhysicalOperatorType::PROJECTION, move(types)), select_list(move(select_list)) {
	}

	vector<unique_ptr<Expression>> select_list;

public:
	void GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state) override;

	unique_ptr<PhysicalOperatorState> GetOperatorState(QueryProfiler *query_profiler) override;
	string ParamsToString() const override;
};

} // namespace duckdb
