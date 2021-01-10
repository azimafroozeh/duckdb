//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/operator/numeric_binary_operators.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/arch.h"
#include "duckdb/common/assert.hpp"
#include <cmath>

namespace duckdb {

struct RENAME(NegateOperator) {
	template <class TA, class TR> static inline TR Operation(TA input) {
		return -input;
	}
};

struct RENAME(DivideOperator) {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		D_ASSERT(right != 0); // this should be checked before!
		return left / right;
	}
};

struct RENAME(ModuloOperator) {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		D_ASSERT(right != 0);
		return left % right;
	}
};

template <> float RENAME(DivideOperator)::Operation(float left, float right);
template <> double RENAME(DivideOperator)::Operation(double left, double right);
template <> hugeint_t RENAME(DivideOperator)::Operation(hugeint_t left, hugeint_t right);
template <> interval_t RENAME(DivideOperator)::Operation(interval_t left, int64_t right);

template <> float RENAME(ModuloOperator)::Operation(float left, float right);
template <> double RENAME(ModuloOperator)::Operation(double left, double right);
template <> hugeint_t RENAME(ModuloOperator)::Operation(hugeint_t left, hugeint_t right);

} // namespace duckdb
