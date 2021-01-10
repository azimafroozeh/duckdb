//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/operator/subtract.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "duckdb/arch.h"
#include "duckdb/common/types.hpp"
#include "duckdb/common/exception.hpp"

namespace duckdb {

struct RENAME(SubtractOperator) {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		return left - right;
	}
};

template <> float RENAME(SubtractOperator)::Operation(float left, float right);
template <> double RENAME(SubtractOperator)::Operation(double left, double right);
template <> interval_t RENAME(SubtractOperator)::Operation(interval_t left, interval_t right);
template <> date_t RENAME(SubtractOperator)::Operation(date_t left, interval_t right);
template <> timestamp_t RENAME(SubtractOperator)::Operation(timestamp_t left, interval_t right);
template <> interval_t RENAME(SubtractOperator)::Operation(timestamp_t left, timestamp_t right);

struct RENAME(TrySubtractOperator) {
	template <class TA, class TB, class TR> static inline bool Operation(TA left, TB right, TR &result) {
		throw InternalException("Unimplemented type for TrySubtractOperator");
	}
};

template <> bool RENAME(TrySubtractOperator)::Operation(int8_t left, int8_t right, int8_t &result);
template <> bool RENAME(TrySubtractOperator)::Operation(int16_t left, int16_t right, int16_t &result);
template <> bool RENAME(TrySubtractOperator)::Operation(int32_t left, int32_t right, int32_t &result);
template <> bool RENAME(TrySubtractOperator)::Operation(int64_t left, int64_t right, int64_t &result);

struct SubtractOperatorOverflowCheck {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		TR result;
		if (!RENAME(TrySubtractOperator)::Operation(left, right, result)) {
			throw OutOfRangeException("Overflow in subtraction of %s (%d - %d)!", TypeIdToString(GetTypeId<TA>()), left,
			                          right);
		}
		return result;
	}
};

struct RENAME(TryDecimalSubtract) {
	template <class TA, class TB, class TR> static inline bool Operation(TA left, TB right, TR &result) {
		throw InternalException("Unimplemented type for TryDecimalSubtract");
	}
};

template <> bool RENAME(TryDecimalSubtract)::Operation(int16_t left, int16_t right, int16_t &result);
template <> bool RENAME(TryDecimalSubtract)::Operation(int32_t left, int32_t right, int32_t &result);
template <> bool RENAME(TryDecimalSubtract)::Operation(int64_t left, int64_t right, int64_t &result);
template <> bool RENAME(TryDecimalSubtract)::Operation(hugeint_t left, hugeint_t right, hugeint_t &result);

struct RENAME(DecimalSubtractOverflowCheck) {
	template <class TA, class TB, class TR> static inline TR Operation(TA left, TB right) {
		TR result;
		if (!RENAME(TryDecimalSubtract)::Operation<TA, TB, TR>(left, right, result)) {
			throw OutOfRangeException("Overflow in subtract of DECIMAL(18) (%d - %d). You might want to add an "
			                          "explicit cast to a bigger decimal.",
			                          left, right);
		}
		return result;
	}
};

template <> hugeint_t RENAME(DecimalSubtractOverflowCheck)::Operation(hugeint_t left, hugeint_t right);

struct RENAME(SubtractTimeOperator) {
	template <class TA, class TB, class TR> static TR Operation(TA left, TB right);
};

template <> dtime_t RENAME(SubtractTimeOperator)::Operation(dtime_t left, interval_t right);

} // namespace duckdb
