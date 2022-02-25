#include "megdnn/oprs.h"

#include "src/common/utils.h"

namespace megdnn {

void Linspace::check_exec(const TensorLayout& dst, size_t workspace_in_bytes) {
    megdnn_assert(dst.ndim == 1 && dst.shape[0] > 0);
    megdnn_assert_contiguous(dst);
    auto required_workspace_in_bytes = get_workspace_in_bytes(dst);
    megdnn_assert(workspace_in_bytes >= required_workspace_in_bytes);
}

}  // namespace megdnn

// vim: syntax=cpp.doxygen
