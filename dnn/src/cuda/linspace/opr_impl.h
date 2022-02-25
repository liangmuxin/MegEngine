#pragma once
#include "megdnn/oprs.h"

namespace megdnn {
namespace cuda {

class LinspaceImpl final : public Linspace {
public:
    using Linspace::Linspace;
    void exec(_megdnn_tensor_out dst, _megdnn_workspace workspace) override;
    size_t get_workspace_in_bytes(const TensorLayout&) override { return 0; }
};

}  // namespace cuda
}  // namespace megdnn
// vim: syntax=cpp.doxygen foldmethod=marker foldmarker=f{{{,f}}}
