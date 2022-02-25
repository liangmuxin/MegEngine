#pragma once
#include "megdnn/oprs.h"

namespace megdnn {
namespace naive {

class FillImpl : public Fill {
public:
    using Fill::Fill;

    void exec(_megdnn_tensor_out dst, _megdnn_workspace workspace) override;
    size_t get_workspace_in_bytes(const TensorLayout&) override { return 0; }

private:
    template <typename ctype>
    void exec_internal(ctype* dst, size_t size);
};

}  // namespace naive
}  // namespace megdnn

// vim: syntax=cpp.doxygen
