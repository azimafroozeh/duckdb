namespace duckdb {

struct PragmaTableOperatorData : public FunctionOperatorData {
	explicit PragmaTableOperatorData(idx_t rows) : rows(rows) {
	}
	idx_t rows;
};

static unique_ptr<FunctionData> PragmaLastProfilingOutputBind(ClientContext &context, vector<Value> &inputs,
                                                              unordered_map<string, Value> &named_parameters,
                                                              vector<LogicalType> &return_types,
                                                              vector<string> &names) {
	names.emplace_back("op_id");
	return_types.push_back(LogicalType::INTEGER);

	names.emplace_back("fun_id");
	return_types.push_back(LogicalType::INTEGER);

	names.emplace_back("description");
	return_types.push_back(LogicalType::VARCHAR);

	names.emplace_back("time");
	return_types.push_back(LogicalType::DOUBLE);

	return make_unique<TableFunctionData>();
}

unique_ptr<FunctionOperatorData> PragmaLastProfilingOutputInit(ClientContext &context, const FunctionData *bind_data,
                                                               vector<column_t> &column_ids,
                                                               TableFilterCollection *filters) {
	return make_unique<PragmaTableOperatorData>(1024);
}

static void SetValue(DataChunk &output, int index, int op_id, int fun_id, string description, double time) {
	output.SetValue(0, index, op_id);
	output.SetValue(1, index, fun_id);
	output.SetValue(2, index, description);
	output.SetValue(3, index, time);
}

static void ExtractExpressions(ExpressionInformation &info, DataChunk &output, int &index, int op_id, int &fun_id,
                               int sample_tuples_count) {
	if (info.hasfunction) {
		SetValue(output, index++, op_id, fun_id++, ", Function: " + info.function_name,
		         double(info.time) / sample_tuples_count);
	}
	if (info.children.empty()) {
		return;
	}
	// extract the children of this node
	for (auto &child : info.children) {
		ExtractExpressions(*child, output, index, op_id, fun_id, sample_tuples_count);
	}
	return;
}

static void PragmaLastProfilingOutputFunction(ClientContext &context, const FunctionData *bind_data_p,
                                              FunctionOperatorData *operator_state, DataChunk &output) {
	auto &state = (PragmaTableOperatorData &)*operator_state;
	if (state.rows > 0) {
		int total_counter = 0;
		int operator_counter = 1;
		SetValue(output, total_counter++, 0, 0, "Query: " + context.query, context.profiler.main_query.Elapsed());
		for (auto op : context.prev_profiler.tree_map) {
			int function_counter = 1;
			SetValue(output, total_counter++, operator_counter, 0, "Operator: " + op.second->name,
			         op.second->info.time);
			if (op.second->info.has_executor) {
				for (auto &info : op.second->info.executors_info->roots) {
					ExtractExpressions(*info, output, total_counter, operator_counter, function_counter,
					                   op.second->info.executors_info->sample_tuples_count);
				}
			}
			operator_counter++;
		}

		state.rows = 0;
		output.SetCardinality(total_counter);
	} else {
		output.SetCardinality(0);
	}
}

void PragmaLastProfilingOutput::RegisterFunction(BuiltinFunctions &set) {
	set.AddFunction(TableFunction("pragma_last_profiling_output", {}, PragmaLastProfilingOutputFunction,
	                              PragmaLastProfilingOutputBind, PragmaLastProfilingOutputInit));
}

} // namespace duckdb
