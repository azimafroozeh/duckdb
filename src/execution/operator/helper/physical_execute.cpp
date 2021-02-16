#include "duckdb/execution/operator/helper/physical_execute.hpp"

namespace duckdb {

void PhysicalExecute::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state_p) {
	D_ASSERT(plan);
	plan->GetChunk(context, chunk, state_p);
}

unique_ptr<PhysicalOperatorState> PhysicalExecute::GetOperatorState(QueryProfiler *query_profiler) {
	return plan->GetOperatorState(query_profiler);
}

} // namespace duckdb
