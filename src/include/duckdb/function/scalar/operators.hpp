//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/function/scalar/operators.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/function/scalar_function.hpp"
#include "duckdb/function/function_set.hpp"
#include "duckdb/arch.h"

namespace duckdb {

struct RENAME(AddFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(SubtractFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(MultiplyFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(DivideFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(ModFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(LeftShiftFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(RightShiftFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(BitwiseAndFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(BitwiseOrFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(BitwiseXorFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

struct RENAME(BitwiseNotFun) {
	static void RegisterFunction(BuiltinFunctions &set);
};

} // namespace duckdb
