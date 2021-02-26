#include "duckdb/function/table/sqlite_functions.hpp"

#include "duckdb/catalog/catalog.hpp"
#include "duckdb/catalog/catalog_entry/table_catalog_entry.hpp"
#include "duckdb/catalog/catalog_entry/view_catalog_entry.hpp"
#include "duckdb/parser/qualified_name.hpp"
#include "duckdb/planner/constraints/bound_not_null_constraint.hpp"
#include "duckdb/planner/constraints/bound_unique_constraint.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/limits.hpp"

#include <algorithm>

namespace duckdb {

struct PragmaTableOperatorData : public FunctionOperatorData {
    PragmaTableOperatorData(idx_t rows) : rows(rows) {
    }
    idx_t rows;
};

static unique_ptr<FunctionData> bind(ClientContext &context, vector<Value> &inputs,
                                                    unordered_map<string, Value> &named_parameters,
                                                    vector<LogicalType> &return_types, vector<string> &names) {
    names.emplace_back("op_id");
    return_types.push_back(LogicalType::INTEGER);

    names.emplace_back("exp_id");
    return_types.push_back(LogicalType::INTEGER);

    names.emplace_back("fun_ic");
    return_types.push_back(LogicalType::INTEGER);

    names.emplace_back("description");
    return_types.push_back(LogicalType::VARCHAR);

    names.emplace_back("time");
    return_types.push_back(LogicalType::DOUBLE);


	return make_unique<TableFunctionData>();
}

unique_ptr<FunctionOperatorData> init(ClientContext &context, const FunctionData *bind_data,
                                                     vector<column_t> &column_ids, TableFilterCollection *filters) {
    return make_unique<PragmaTableOperatorData>(1);
}

static void SetValue(DataChunk &output, int index,  int op_id, int exp_id, int fun_id, string description, double time){
    output.SetValue(0, index, op_id);
    output.SetValue(1, index, exp_id);
    output.SetValue(2, index, fun_id);
    output.SetValue(3, index, description);
    output.SetValue(4, index, time);
}

static void ExtractExpressions(ExpressionInformation &info, DataChunk &output, int index, int operator_counter) {
    if (info.children.empty()) {
        return;
    }
    // extract the children of this node
    for (auto &child : info.children) {

    }
    return;
}

static void function1(ClientContext &context, const FunctionData *bind_data_p,
                                    FunctionOperatorData *operator_state, DataChunk &output) {
    auto &state = (PragmaTableOperatorData &)*operator_state;
    if(state.rows > 0) {
		int total_counter = 0;
        int operator_counter = 1;
		int function_counter = 0;
		SetValue(output, total_counter++, 0, 0, 0, "main_query", context.profiler.main_query.Elapsed());
		for(auto op: context.prev_profiler.tree_map){
			int expression_counter = 0;
            SetValue(output, total_counter++, operator_counter++, 0, 0, op.second->name , op.second->info.time);
		}

		state.rows = 0;
        output.SetCardinality(total_counter);
	}
	else{
		output.SetCardinality(0);
	}
}

void PragmaLastProfilingOutput::RegisterFunction(BuiltinFunctions &set) {
    set.AddFunction(TableFunction("pragma_last_profiling_output", {}, function1,
                                  bind, init));
}

} // namespace duckdb
