#pragma once
#include "megdnn/oprs.h"

namespace megdnn {
namespace arm_common {

class CvtColorImpl : public CvtColor {
private:
    template <typename T>
    void cvt_color_exec(_megdnn_tensor_in src, _megdnn_tensor_out dst);

public:
    using CvtColor::CvtColor;

    void exec(
            _megdnn_tensor_in src, _megdnn_tensor_out dst,
            _megdnn_workspace workspace) override;

    size_t get_workspace_in_bytes(const TensorLayout&, const TensorLayout&) override {
        return 0;
    }
};

}  // namespace arm_common
}  // namespace megdnn

// vim: syntax=cpp.doxygen
