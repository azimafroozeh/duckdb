#include "duckdb/execution/operator/projection/physical_projection.hpp"

#include "duckdb/execution/expression_executor.hpp"

namespace duckdb {

class PhysicalProjectionState : public PhysicalOperatorState {
public:
	PhysicalProjectionState(QueryProfiler *query_profiler, PhysicalOperator &op, PhysicalOperator *child, vector<unique_ptr<Expression>> &expressions)
	    : PhysicalOperatorState(op, child), executor(query_profiler, expressions) {
		D_ASSERT(child);
	}

	ExpressionExecutor executor;
};

void PhysicalProjection::GetChunkInternal(ExecutionContext &context, DataChunk &chunk, PhysicalOperatorState *state_p) {
	auto state = reinterpret_cast<PhysicalProjectionState *>(state_p);

	// get the next chunk from the child
	children[0]->GetChunk(context, state->child_chunk, state->child_state.get());
	if (state->child_chunk.size() == 0) {
		return;
	}

	state->executor.Execute(state->child_chunk, chunk);
}

unique_ptr<PhysicalOperatorState> PhysicalProjection::GetOperatorState(QueryProfiler *query_profiler) {
	return make_unique<PhysicalProjectionState>(query_profiler, *this, children[0].get(), select_list);
}

string PhysicalProjection::ParamsToString() const {
	string extra_info;
	for (auto &expr : select_list) {
		extra_info += expr->GetName() + "\n";
	}
	return extra_info;
}

} // namespace duckdb
