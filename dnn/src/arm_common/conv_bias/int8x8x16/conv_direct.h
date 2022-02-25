#pragma once

#include "src/arm_common/conv_bias/opr_impl.h"

#include <cstddef>
#include <cstdint>

namespace megdnn {
namespace arm_common {
namespace conv_bias {

template <bool add_to_dst>
void conv_direct_2x2_sc_int8_int8_int16(
        const int8_t* src, const int8_t* filter, int16_t* dst, size_t IH, size_t IW,
        size_t OH, size_t OW, size_t PH, size_t PW);
template <bool add_to_dst>
void conv_direct_3x3_sc_int8_int8_int16(
        const int8_t* src, const int8_t* filter, int16_t* dst, size_t IH, size_t IW,
        size_t OH, size_t OW, size_t PH, size_t PW);
template <bool add_to_dst>
void conv_direct_5x5_sc_int8_int8_int16(
        const int8_t* src, const int8_t* filter, int16_t* dst, size_t IH, size_t IW,
        size_t OH, size_t OW, size_t PH, size_t PW);
}  // namespace conv_bias
}  // namespace arm_common
}  // namespace megdnn

// vim: syntax=cpp.doxygen
