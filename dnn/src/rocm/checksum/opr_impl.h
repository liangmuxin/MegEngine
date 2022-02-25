#pragma once

#include "megdnn/oprs.h"
#include "src/rocm/utils.h"

namespace megdnn {
namespace rocm {

class ChecksumForwardImpl final : public ChecksumForward {
public:
    using ChecksumForward::ChecksumForward;

    size_t get_workspace_in_bytes(const TensorLayout&) override;

    bool is_thread_safe() const override { return true; }

    Result exec(_megdnn_tensor_in data, _megdnn_workspace workspace) override;
};

}  // namespace rocm
}  // namespace megdnn

// vim: syntax=cpp.doxygen
