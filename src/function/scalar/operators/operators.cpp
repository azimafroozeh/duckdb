#include "duckdb/function/scalar/operators.hpp"
#include "duckdb/common/exception.hpp"


namespace duckdb {

void BuiltinFunctions::RENAME(RegisterOperators)() {
	Register<RENAME(AddFun)>();
	Register<RENAME(SubtractFun)>();
	Register<RENAME(MultiplyFun)>();
	Register<RENAME(DivideFun)>();
	Register<RENAME(ModFun)>();
	Register<RENAME(LeftShiftFun)>();
	Register<RENAME(RightShiftFun)>();
	Register<RENAME(BitwiseAndFun)>();
	Register<RENAME(BitwiseOrFun)>();
	Register<RENAME(BitwiseXorFun)>();
	Register<RENAME(BitwiseNotFun)>();
}

} // namespace duckdb
