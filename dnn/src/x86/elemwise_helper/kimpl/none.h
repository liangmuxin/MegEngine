#pragma once

#include "src/x86/elemwise_helper/kimpl/op_unary_base.h"
#include "src/x86/quantized_converter.h"
#include "src/x86/utils.h"

namespace megdnn {
namespace x86 {

template <SIMDType simd_type, typename src_ctype, typename dst_ctype = src_ctype>
struct NoneOpBase : UnaryOpBase<simd_type, src_ctype, dst_ctype> {
    using UnaryOpBase<simd_type, src_ctype, dst_ctype>::UnaryOpBase;
    void operator()(const src_ctype& src, dst_ctype* dst) const {
        *dst = operator()(src);
    }
    dst_ctype operator()(const src_ctype& src) const { return src; }
};

template <SIMDType simd_type, typename src_ctype, typename dst_type = src_ctype>
struct NoneOp;

#define OP(                                                                            \
        _ctype, _simd_type, _simd_target, _simd_data_type, _simd_data_type2,           \
        _ptr_type, _func_prefix, _func_suffix1, _func_suffix2, _simd_width)            \
    template <>                                                                        \
    struct NoneOp<_simd_type, _ctype> : NoneOpBase<_simd_type, _ctype> {               \
        using NoneOpBase::NoneOpBase;                                                  \
        using NoneOpBase::operator();                                                  \
        constexpr static size_t SIMD_WIDTH = _simd_width;                              \
        MEGDNN_ATTRIBUTE_TARGET(_simd_target)                                          \
        void operator()(const _simd_data_type2& src, _ctype* dst) const {              \
            auto vitem = operator()(src);                                              \
            _##_func_prefix##_storeu_##_func_suffix2(                                  \
                    reinterpret_cast<_ptr_type*>(dst), vitem.val[0]);                  \
            _##_func_prefix##_storeu_##_func_suffix2(                                  \
                    reinterpret_cast<_ptr_type*>(dst + SIMD_WIDTH), vitem.val[1]);     \
        }                                                                              \
        MEGDNN_ATTRIBUTE_TARGET(_simd_target)                                          \
        _simd_data_type2 operator()(const _simd_data_type2& src) const { return src; } \
        MEGDNN_ATTRIBUTE_TARGET(_simd_target)                                          \
        _simd_data_type operator()(const _simd_data_type& src) const { return src; }   \
    };
OP(dt_float32, SIMDType::SSE4_2, "sse4.2", __m128, __m128x2, float, mm, ps, ps, 4)
OP(dt_int32, SIMDType::SSE4_2, "sse4.2", __m128i, __m128ix2, __m128i, mm, epi32, si128,
   4)
OP(dt_int16, SIMDType::SSE4_2, "sse4.2", __m128i, __m128ix2, __m128i, mm, epi16, si128,
   8)
OP(dt_int8, SIMDType::SSE4_2, "sse4.2", __m128i, __m128ix2, __m128i, mm, epi8, si128,
   16)

OP(dt_float32, SIMDType::AVX2, "avx2", __m256, __m256x2, float, mm256, ps, ps, 8)
OP(dt_int32, SIMDType::AVX2, "avx2", __m256i, __m256ix2, __m256i, mm256, epi32, si256,
   8)
OP(dt_int16, SIMDType::AVX2, "avx2", __m256i, __m256ix2, __m256i, mm256, epi16, si256,
   16)
OP(dt_int8, SIMDType::AVX2, "avx2", __m256i, __m256ix2, __m256i, mm256, epi8, si256, 32)
#undef OP
#define OP(_ctype, _simd_type)                                           \
    template <>                                                          \
    struct NoneOp<_simd_type, _ctype> : NoneOpBase<_simd_type, _ctype> { \
        using NoneOpBase::NoneOpBase;                                    \
        using NoneOpBase::operator();                                    \
    };
OP(dt_float32, SIMDType::NONE)
#undef OP

}  // namespace x86
}  // namespace megdnn

// vim: syntax=cpp.doxygen
