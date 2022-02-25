#pragma once

#include "src/aarch64/conv_bias/opr_impl.h"
#include "src/fallback/conv_bias/opr_impl.h"

namespace megdnn {
namespace aarch64 {

using FallbackConvBiasImpl = fallback::ConvBiasImpl;
/* ===================== stride-2 algo ===================== */

class ConvBiasImpl::AlgoF32DirectStride2 final : public AlgoBase {
    SmallVector<NCBKern> get_kimpls(const NCBKernSizeParam& param) const;

public:
    AlgoAttribute attribute() const override { return AlgoAttribute::REPRODUCIBLE; }
    const char* name() const override { return "ARMV8F32STRD2"; }

    bool usable(
            const NCBKernSizeParam& param,
            AlgoSelectionStrategy algo_selection_strategy) const override;

    size_t get_workspace(const NCBKernSizeParam& param) const override;

    SmallVector<NCBKern> dispatch_kerns(const NCBKernSizeParam&) const override;

    ConvAlgoTypePack get_algo_type() const override {
        return {AlgoDataType::FLOAT32, AlgoCategory::DIRECT};
    }
    MEGDNN_DECL_ALGO_TYPE(AARCH64_DIRECT_STRD2_FP32)
};

}  // namespace aarch64
}  // namespace megdnn

// vim: syntax=cpp.doxygen
