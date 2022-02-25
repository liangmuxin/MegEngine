#pragma once
#include "megdnn/oprs.h"

namespace megdnn {
namespace naive {

class FlipImpl : public Flip {
public:
    using Flip::Flip;

    void exec(
            _megdnn_tensor_in src, _megdnn_tensor_out dst,
            _megdnn_workspace workspace) override;

    size_t get_workspace_in_bytes(const TensorLayout&, const TensorLayout&) override {
        return 0;
    }

private:
    template <typename T>
    void exec_internal(_megdnn_tensor_in src, _megdnn_tensor_out dst);
};

}  // namespace naive
}  // namespace megdnn

// vim: syntax=cpp.doxygen
