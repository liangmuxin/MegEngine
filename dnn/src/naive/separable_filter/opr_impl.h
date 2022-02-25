#pragma once
#include "megdnn/oprs.h"
namespace megdnn {
namespace naive {

class SeparableFilterForwardImpl : public SeparableFilterForward {
public:
    using SeparableFilterForward::SeparableFilterForward;
    void exec(
            _megdnn_tensor_in src, _megdnn_tensor_in filter_x,
            _megdnn_tensor_in filter_y, _megdnn_tensor_out dst,
            _megdnn_workspace workspace) override;

    size_t get_workspace_in_bytes(
            const TensorLayout&, const TensorLayout&, const TensorLayout&,
            const TensorLayout&) override {
        return 0;
    }
};

}  // namespace naive
}  // namespace megdnn
// vim: syntax=cpp.doxygen
