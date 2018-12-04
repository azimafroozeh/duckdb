#include "execution/operator/data_modification/physical_delete.hpp"
#include "execution/expression_executor.hpp"

#include "main/client_context.hpp"

#include "storage/data_table.hpp"

using namespace duckdb;
using namespace std;

void PhysicalDelete::_GetChunk(ClientContext &context, DataChunk &chunk,
                               PhysicalOperatorState *state) {
	chunk.Reset();

	if (state->finished) {
		return;
	}

	int64_t deleted_count = 0;
	while (true) {
		children[0]->GetChunk(context, state->child_chunk,
		                      state->child_state.get());
		if (state->child_chunk.size() == 0) {
			break;
		}
		// delete data in the base table
		// the row ids are given to us as the last column of the child chunk
		table.Delete(
		    tableref, context,
		    state->child_chunk.data[state->child_chunk.column_count - 1]);
		deleted_count += state->child_chunk.size();
	}

	chunk.data[0].count = 1;
	chunk.data[0].SetValue(0, Value::BIGINT(deleted_count));

	state->finished = true;
}

unique_ptr<PhysicalOperatorState>
PhysicalDelete::GetOperatorState(ExpressionExecutor *parent_executor) {
	return make_unique<PhysicalOperatorState>(children[0].get(),
	                                          parent_executor);
}