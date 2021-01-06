#include "duckdb/function/scalar/math_functions.hpp"
#include "iostream"
#include <immintrin.h>
#include "duckdb/common/dispatch.hpp"



namespace duckdb {

static void plus_default(DataChunk &args, ExpressionState &state, Vector &result) {
    auto result_data = FlatVector::GetData<int32_t>(result);
    auto data1 = FlatVector::GetData<int32_t>(args.data[0]);
    auto data2 = FlatVector::GetData<int32_t>(args.data[1]);
    __m256i r;
    __m256i l;
    __m256i tmp;
    for (int i = 0; i < 1024 ; ++i){
		result_data[i] = data1[i] + data2[i];
    }
	std::cout << "default";
    auto cpu_info = arrow::internal::CpuInfo::GetInstance();
	std::cout << cpu_info->num_cores();
}

#if defined(ARROW_HAVE_RUNTIME_AVX2)
static void plus_avx2(DataChunk &args, ExpressionState &state, Vector &result) {
    auto result_data = FlatVector::GetData<int32_t>(result);
    auto data1 = FlatVector::GetData<int32_t>(args.data[0]);
    auto data2 = FlatVector::GetData<int32_t>(args.data[1]);
    __m256i r;
    __m256i l;
	__m256i tmp;
	for (int i = 0; i < 1024 ; i = i + 8){
		r = _mm256_loadu_si256(reinterpret_cast<const __m256i_u *>(data1 + i));
        l = _mm256_loadu_si256(reinterpret_cast<const __m256i_u *>(data2 + i));
		tmp = _mm256_add_epi32(r, l);
		_mm256_storeu_si256(reinterpret_cast<__m256i_u *>(result_data + i), tmp);
	}
	std::cout << "avx2";
}
#endif

#if defined(ARROW_HAVE_RUNTIME_AVX512)
static void plus_avx512(DataChunk &args, ExpressionState &state, Vector &result) {
    auto result_data = FlatVector::GetData<int32_t>(result);
    auto data1 = FlatVector::GetData<int32_t>(args.data[0]);
    auto data2 = FlatVector::GetData<int32_t>(args.data[1]);
    __m256i r;
    __m256i l;
    __m256i tmp;
    for (int i = 0; i < 1024 ; i = i + 16){
        r = _mm256_loadu_si512(reinterpret_cast<const __m512i_u *>(data1 + i));
        l = _mm256_loadu_si512(reinterpret_cast<const __m512i_u *>(data2 + i));
        tmp = _mm256_add_epi32(r, l);
        _mm256_storeu_si256(reinterpret_cast<__m256i_u *>(result_data + i), tmp);
    }
	std::cout << "avx512";
}
#endif

struct PLusDynamicFunction {
    using FunctionType = decltype(&plus_default);

    static std::vector<std::pair<arrow::internal::DispatchLevel, FunctionType>> implementations() {
        return {
            { arrow::internal::DispatchLevel::NONE, plus_default }
#if defined(ARROW_HAVE_RUNTIME_AVX2)
            , { DispatchLevel::AVX2, plus_avx2 }
#endif
#if defined(ARROW_HAVE_RUNTIME_AVX512)
            , { DispatchLevel::AVX512, plus_avx512 }
#endif
        };
    }
};

void plus(DataChunk &args, ExpressionState &state, Vector &result) {
    static arrow::internal::DynamicDispatch<PLusDynamicFunction> dispatch;
    return dispatch.func(args, state, result);
}



static void plustest(DataChunk &args, ExpressionState &state, Vector &result) {
    auto result_data = FlatVector::GetData<int32_t>(result);
    auto data1 = FlatVector::GetData<int32_t>(args.data[0]);
    auto data2 = FlatVector::GetData<int32_t>(args.data[1]);
    for (int i = 0; i < 1024 ; ++i){
		result_data[i] = data1[i] + data2[i];
    }
}


void PlusFun::RegisterFunction(BuiltinFunctions &set) {
    // hex
    ScalarFunctionSet functions("plus");
    functions.AddFunction(ScalarFunction({LogicalType::INTEGER, LogicalType::INTEGER}, LogicalType::INTEGER, plus));

    set.AddFunction(functions);
}





} // namespace duckdb
