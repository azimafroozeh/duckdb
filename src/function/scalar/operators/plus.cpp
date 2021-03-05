//
// Created by Azim Afroozeh on 3/5/21.
//

#include "duckdb/function/scalar/math_functions.hpp"
#include "iostream"
#include <duckdb/function/scalar/operators.hpp>
#include <immintrin.h>

namespace duckdb {

template<typename T>
static void plus_default(DataChunk &args, ExpressionState &state, Vector &result) {
	auto result_data = FlatVector::GetData<T>(result);
	auto data1 = FlatVector::GetData<T>(args.data[0]);
	auto data2 = FlatVector::GetData<T>(args.data[1]);
	for (int i = 0; i < 1024; ++i) {
		result_data[i] = data1[i] + data2[i];
	}
}
//
//#ifdef AVX2
// static void plus_avx2(DataChunk &args, ExpressionState &state, Vector &result) {
//    auto result_data = FlatVector::GetData<uint8_t>(result);
//    auto data1 = FlatVector::GetData<uint8_t>(args.data[0]);
//    auto data2 = FlatVector::GetData<int8_t>(args.data[1]);
//    __m256i r;
//    __m256i l;
//    __m256i tmp;
//    for (int i = 0; i < 1024 ; i = i + 32){
//        r = _mm256_loadu_si256(reinterpret_cast<const __m256i_u *>(data1 + i));
//        l = _mm256_loadu_si256(reinterpret_cast<const __m256i_u *>(data2 + i));
//        tmp = _mm256_add_epi8(r, l);
//        _mm256_storeu_si256(reinterpret_cast<__m256i_u *>(result_data + i), tmp);
//    }
//}
//#else
//static void plus_avx2(DataChunk &args, ExpressionState &state, Vector &result) {
//	std::cout << "here";
//}
//#endif
//
// static void plus_avx512(DataChunk &args, ExpressionState &state, Vector &result) {
//    auto result_data = FlatVector::GetData<int8_t>(result);
//    auto data1 = FlatVector::GetData<int8_t>(args.data[0]);
//    auto data2 = FlatVector::GetData<int8_t>(args.data[1]);
//    __m512i r;
//    __m512i l;
//    __m512i tmp;
//    for (int i = 0; i < 1024 ; i = i + 64){
//        r = _mm512_loadu_si512(reinterpret_cast<const __m512i_u *>(data1 + i));
//        l = _mm512_loadu_si512(reinterpret_cast<const __m512i_u *>(data2 + i));
//        tmp = _mm512_add_epi8(r, l);
//        _mm512_storeu_epi8(reinterpret_cast<__m256i_u *>(result_data + i), tmp);
//    }
//}

void SET_ARCH(Plus)::RegisterFunction(BuiltinFunctions &set) {
	// hex
	ScalarFunctionSet functions("plus");
	functions.AddFunction(
	    ScalarFunction({LogicalType::TINYINT, LogicalType::TINYINT}, LogicalType::TINYINT, plus_default<uint8_t>));
    functions.AddFunction(
        ScalarFunction({LogicalType::SMALLINT, LogicalType::SMALLINT}, LogicalType::SMALLINT, plus_default<uint16_t>));
    functions.AddFunction(
        ScalarFunction({LogicalType::INTEGER, LogicalType::INTEGER}, LogicalType::INTEGER, plus_default<uint32_t>));
    functions.AddFunction(
        ScalarFunction({LogicalType::BIGINT, LogicalType::BIGINT}, LogicalType::BIGINT, plus_default<uint64_t>));

	set.AddFunction(functions);
}
}