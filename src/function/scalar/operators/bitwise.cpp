#include "duckdb/function/scalar/operators.hpp"
#include "duckdb/common/vector_operations/vector_operations.hpp"
#include "duckdb/function/functions.hpp"

namespace duckdb {

template <class OP>
static scalar_function_t GetScalarIntegerUnaryFunction(const LogicalType &type) {
	scalar_function_t function;
	switch (type.id()) {
	case LogicalTypeId::TINYINT:
		function = &ScalarFunction::UnaryFunction<int8_t, int8_t, OP>;
		break;
	case LogicalTypeId::SMALLINT:
		function = &ScalarFunction::UnaryFunction<int16_t, int16_t, OP>;
		break;
	case LogicalTypeId::INTEGER:
		function = &ScalarFunction::UnaryFunction<int32_t, int32_t, OP>;
		break;
	case LogicalTypeId::BIGINT:
		function = &ScalarFunction::UnaryFunction<int64_t, int64_t, OP>;
		break;
	case LogicalTypeId::UTINYINT:
		function = &ScalarFunction::UnaryFunction<uint8_t, uint8_t, OP>;
		break;
	case LogicalTypeId::USMALLINT:
		function = &ScalarFunction::UnaryFunction<uint16_t, uint16_t, OP>;
		break;
	case LogicalTypeId::UINTEGER:
		function = &ScalarFunction::UnaryFunction<uint32_t, uint32_t, OP>;
		break;
	case LogicalTypeId::UBIGINT:
		function = &ScalarFunction::UnaryFunction<uint64_t, uint64_t, OP>;
		break;
	case LogicalTypeId::HUGEINT:
		function = &ScalarFunction::UnaryFunction<hugeint_t, hugeint_t, OP>;
		break;
	default:
		throw NotImplementedException("Unimplemented type for GetScalarIntegerUnaryFunction");
	}
	return function;
}

template <class OP>
static scalar_function_t GetScalarIntegerBinaryFunction(const LogicalType &type) {
	scalar_function_t function;
	switch (type.id()) {
	case LogicalTypeId::TINYINT:
		function = &ScalarFunction::BinaryFunction<int8_t, int8_t, int8_t, OP>;
		break;
	case LogicalTypeId::SMALLINT:
		function = &ScalarFunction::BinaryFunction<int16_t, int16_t, int16_t, OP>;
		break;
	case LogicalTypeId::INTEGER:
		function = &ScalarFunction::BinaryFunction<int32_t, int32_t, int32_t, OP>;
		break;
	case LogicalTypeId::BIGINT:
		function = &ScalarFunction::BinaryFunction<int64_t, int64_t, int64_t, OP>;
		break;
	case LogicalTypeId::UTINYINT:
		function = &ScalarFunction::BinaryFunction<uint8_t, uint8_t, uint8_t, OP>;
		break;
	case LogicalTypeId::USMALLINT:
		function = &ScalarFunction::BinaryFunction<uint16_t, uint16_t, uint16_t, OP>;
		break;
	case LogicalTypeId::UINTEGER:
		function = &ScalarFunction::BinaryFunction<uint32_t, uint32_t, uint32_t, OP>;
		break;
	case LogicalTypeId::UBIGINT:
		function = &ScalarFunction::BinaryFunction<uint64_t, uint64_t, uint64_t, OP>;
		break;
	case LogicalTypeId::HUGEINT:
		function = &ScalarFunction::BinaryFunction<hugeint_t, hugeint_t, hugeint_t, OP>;
		break;
	default:
		throw NotImplementedException("Unimplemented type for GetScalarIntegerBinaryFunction");
	}
	return function;
}

//===--------------------------------------------------------------------===//
// & [bitwise_and]
//===--------------------------------------------------------------------===//
struct BitwiseANDOperator {
	template <class TA, class TB, class TR>
	static inline TR Operation(TA left, TB right) {
		return left & right;
	}
};

void SET_ARCH(BitwiseAndFun)::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunctionSet functions("&");
	for (auto &type : LogicalType::INTEGRAL) {
		functions.AddFunction(
		    ScalarFunction({type, type}, type, GetScalarIntegerBinaryFunction<BitwiseANDOperator>(type)));
	}
	set.AddFunction(functions);
}

//===--------------------------------------------------------------------===//
// | [bitwise_or]
//===--------------------------------------------------------------------===//
struct BitwiseOROperator {
	template <class TA, class TB, class TR>
	static inline TR Operation(TA left, TB right) {
		return left | right;
	}
};

void SET_ARCH(BitwiseOrFun)::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunctionSet functions("|");
	for (auto &type : LogicalType::INTEGRAL) {
		functions.AddFunction(
		    ScalarFunction({type, type}, type, GetScalarIntegerBinaryFunction<BitwiseOROperator>(type)));
	}
	set.AddFunction(functions);
}

//===--------------------------------------------------------------------===//
// # [bitwise_xor]
//===--------------------------------------------------------------------===//
struct BitwiseXOROperator {
	template <class TA, class TB, class TR>
	static inline TR Operation(TA left, TB right) {
		return left ^ right;
	}
};

void SET_ARCH(BitwiseXorFun)::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunctionSet functions("#");
	for (auto &type : LogicalType::INTEGRAL) {
		functions.AddFunction(
		    ScalarFunction({type, type}, type, GetScalarIntegerBinaryFunction<BitwiseXOROperator>(type)));
	}
	set.AddFunction(functions);
}

//===--------------------------------------------------------------------===//
// << [bitwise_left_shift]
//===--------------------------------------------------------------------===//
template <class T>
bool ShiftInRange(T shift) {
	return shift >= 0 && shift < T(sizeof(T) * 8);
}

struct BitwiseShiftLeftOperator {
	template <class TA, class TB, class TR>
	static inline TR Operation(TA input, TB shift) {
		return ShiftInRange(shift) ? input << shift : 0;
	}
};

void SET_ARCH(LeftShiftFun)::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunctionSet functions("<<");
	for (auto &type : LogicalType::INTEGRAL) {
		functions.AddFunction(
		    ScalarFunction({type, type}, type, GetScalarIntegerBinaryFunction<BitwiseShiftLeftOperator>(type)));
	}
	set.AddFunction(functions);
}

//===--------------------------------------------------------------------===//
// >> [bitwise_right_shift]
//===--------------------------------------------------------------------===//
struct BitwiseShiftRightOperator {
	template <class TA, class TB, class TR>
	static inline TR Operation(TA input, TB shift) {
		return ShiftInRange(shift) ? input >> shift : 0;
	}
};

void SET_ARCH(RightShiftFun)::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunctionSet functions(">>");
	for (auto &type : LogicalType::INTEGRAL) {
		functions.AddFunction(
		    ScalarFunction({type, type}, type, GetScalarIntegerBinaryFunction<BitwiseShiftRightOperator>(type)));
	}
	set.AddFunction(functions);
}

//===--------------------------------------------------------------------===//
// ~ [bitwise_not]
//===--------------------------------------------------------------------===//
struct BitwiseNotOperator {
	template <class TA, class TR>
	static inline TR Operation(TA input) {
		return ~input;
	}
};

void SET_ARCH(BitwiseNotFun)::RegisterFunction(BuiltinFunctions &set) {
	ScalarFunctionSet functions("~");
	for (auto &type : LogicalType::INTEGRAL) {
		functions.AddFunction(ScalarFunction({type}, type, GetScalarIntegerUnaryFunction<BitwiseNotOperator>(type)));
	}
	set.AddFunction(functions);
}

ScalarFunctionSet SET_ARCH_FRONT(Functions)::GetFunctions() {
	ScalarFunctionSet functions("~");
	for (auto &type : LogicalType::INTEGRAL) {
		functions.AddFunction(ScalarFunction({type}, type, GetScalarIntegerUnaryFunction<BitwiseNotOperator>(type)));
	}
	return functions;
}
} // namespace duckdb
