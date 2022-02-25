#pragma once
#include <cstddef>
#include <cstdint>
#include "src/common/utils.h"

namespace megdnn {
namespace arm_common {

void do_max_pooling_3x3_s2x2_int8_NEON(
        const int8_t* src, int8_t* dst, size_t IH, size_t IW, size_t OH, size_t OW,
        size_t PH, size_t PW, const WorkspaceBundle& boudle);

void do_max_pooling_3x3_s2x2_uint8_NEON(
        const uint8_t* src, uint8_t* dst, size_t IH, size_t IW, size_t OH, size_t OW,
        size_t PH, size_t PW, const WorkspaceBundle& boudle);

}  // namespace arm_common
}  // namespace megdnn

// vim: syntax=cpp.doxygen
