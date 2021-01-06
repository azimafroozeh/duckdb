#include "duckdb/function/scalar/math_functions.hpp"
#include "iostream"
#include <immintrin.h>




namespace duckdb {

static void plus_default(DataChunk &args, ExpressionState &state, Vector &result) {
    auto result_data = FlatVector::GetData<int32_t>(result);
    auto data1 = FlatVector::GetData<int32_t>(args.data[0]);
    auto data2 = FlatVector::GetData<int32_t>(args.data[1]);
    for (int i = 0; i < 1024 ; ++i){
        result_data[i] = data1[i] + data2[i];
    }
}


static void plus_avx2(DataChunk &args, ExpressionState &state, Vector &result) {
    auto result_data = FlatVector::GetData<int8_t>(result);
    auto data1 = FlatVector::GetData<int8_t>(args.data[0]);
    auto data2 = FlatVector::GetData<int8_t>(args.data[1]);
    __m256i r;
    __m256i l;
	__m256i tmp;
	for (int i = 0; i < 1024 ; i = i + 32){
		r = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(data1 + i));
        l = _mm256_loadu_si256(reinterpret_cast<const __m256i *>(data2 + i));
		tmp = _mm256_add_epi8(r, l);
		_mm256_storeu_si256(reinterpret_cast<__m256i_u *>(result_data + i), tmp);
	}
}

static void plus_avx512(DataChunk &args, ExpressionState &state, Vector &result) {
    auto result_data = FlatVector::GetData<int8_t>(result);
    auto data1 = FlatVector::GetData<int8_t>(args.data[0]);
    auto data2 = FlatVector::GetData<int8_t>(args.data[1]);
    __m512i r;
    __m512i l;
    __m512i tmp;
    for (int i = 0; i < 1024 ; i = i + 64){
        r = _mm512_loadu_si512(reinterpret_cast<const __m512i *>(data1 + i));
        l = _mm512_loadu_si512(reinterpret_cast<const __m512i *>(data2 + i));
        tmp = _mm512_add_epi8(r, l);
        _mm512_storeu_epi8(reinterpret_cast<__m512i *>(result_data + i), tmp);
    }
}


void PlusFun::RegisterFunction(BuiltinFunctions &set) {
    // hex
    ScalarFunctionSet functions("plus");
    functions.AddFunction(ScalarFunction({LogicalType::TINYINT, LogicalType::TINYINT}, LogicalType::TINYINT, plus_avx512));

    set.AddFunction(functions);
}





} // namespace duckdb