#pragma once

#include "src/arm_common/conv_bias/opr_impl.h"
#if MGB_ENABLE_DOT
namespace megdnn {
namespace arm_common {
namespace direct_dotprod_int8_stride1 {
using NCBKernSizeParam = fallback::ConvBiasImpl::NCBKernSizeParam;
using NCBKernParam = fallback::ConvBiasImpl::NCBKernParam;
using NCBKernIndex = fallback::ConvBiasImpl::NCBKernIndex;

using conv_fun = std::function<void(
        const WorkspaceBundle& bundle, const NCBKernParam& kern_param,
        const NCBKernIndex& ncb_index, const CpuNDRange& workspace_ids)>;

bool can_conv_direct_stride1_int8(const NCBKernSizeParam& param);

WorkspaceBundle get_bundle(const NCBKernSizeParam& param, bool m_large_group);

void copy_padding_kern(
        const WorkspaceBundle& bundle, const NCBKernParam& kern_param,
        const NCBKernIndex& ncb_index, const CpuNDRange& workspace_ids);

template <size_t filter, BiasMode bias_mode, typename Op>
void do_conv_kern(
        const WorkspaceBundle& bundle, const NCBKernParam& kern_param,
        const NCBKernIndex& ncb_index, const CpuNDRange& workspace_ids);

SmallVector<ConvBiasImpl::NCBKern> get_kimpls(const NCBKernSizeParam& param, bool);
}  // namespace direct_dotprod_int8_stride1
}  // namespace arm_common
}  // namespace megdnn
#endif
// vim: syntax=cpp.doxygen
