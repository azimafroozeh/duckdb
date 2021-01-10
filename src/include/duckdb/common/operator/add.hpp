//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/operator/add.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once
#include "duckdb/arch.h"
#include "duckdb/common/types.hpp"
#include "duckdb/common/exception.hpp"

namespace duckdb {

struct RENAME(AddOperator) {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		return left + right;
	}
};

template <> float RENAME(AddOperator)::Operation(float left, float right);
template <> double RENAME(AddOperator)::Operation(double left, double right);
template <> interval_t RENAME(AddOperator)::Operation(interval_t left, interval_t right);
template <> date_t RENAME(AddOperator)::Operation(date_t left, interval_t right);
template <> date_t RENAME(AddOperator)::Operation(interval_t left, date_t right);
template <> timestamp_t RENAME(AddOperator)::Operation(timestamp_t left, interval_t right);
template <> timestamp_t RENAME(AddOperator)::Operation(interval_t left, timestamp_t right);

struct RENAME(TryAddOperator) {
	template <class TA, class TB, class TR> static inline bool Operation(TA left, TB right, TR &result) {
		throw InternalException("Unimplemented type for TryAddOperator");
	}
};

template <> bool RENAME(TryAddOperator)::Operation(int8_t left, int8_t right, int8_t &result);
template <> bool RENAME(TryAddOperator)::Operation(int16_t left, int16_t right, int16_t &result);
template <> bool RENAME(TryAddOperator)::Operation(int32_t left, int32_t right, int32_t &result);
template <> bool RENAME(TryAddOperator)::Operation(int64_t left, int64_t right, int64_t &result);

struct AddOperatorOverflowCheck {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		TR result;
		if (!RENAME(TryAddOperator)::Operation(left, right, result)) {
			throw OutOfRangeException("Overflow in addition of %s (%d + %d)!", TypeIdToString(GetTypeId<TA>()), left,
			                          right);
		}
		return result;
	}
};

struct RENAME(TryDecimalAdd) {
	template <class TA, class TB, class TR> static inline bool Operation(TA left, TB right, TR &result) {
		throw InternalException("Unimplemented type for TryDecimalAdd");
	}
};

template <> bool RENAME(TryDecimalAdd)::Operation(int16_t left, int16_t right, int16_t &result);
template <> bool RENAME(TryDecimalAdd)::Operation(int32_t left, int32_t right, int32_t &result);
template <> bool RENAME(TryDecimalAdd)::Operation(int64_t left, int64_t right, int64_t &result);
template <> bool RENAME(TryDecimalAdd)::Operation(hugeint_t left, hugeint_t right, hugeint_t &result);

struct RENAME(DecimalAddOverflowCheck) {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		TR result;
		if (!RENAME(TryDecimalAdd)::Operation<TA, TB, TR>(left, right, result)) {
			throw OutOfRangeException("Overflow in addition of DECIMAL(18) (%d + %d). You might want to add an "
			                          "explicit cast to a bigger decimal.",
			                          left, right);
		}
		return result;
	}
};

template <> hugeint_t RENAME(DecimalAddOverflowCheck)::Operation(hugeint_t left, hugeint_t right);

struct RENAME(AddTimeOperator) {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right);
};

template <> dtime_t RENAME(AddTimeOperator)::Operation(dtime_t left, interval_t right);
template <> dtime_t RENAME(AddTimeOperator)::Operation(interval_t left, dtime_t right);

} // namespace duckdb
