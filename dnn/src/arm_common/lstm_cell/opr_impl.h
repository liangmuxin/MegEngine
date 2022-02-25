#pragma once

#include "src/common/utils.h"
#include "src/naive/lstm_cell/opr_impl.h"

namespace megdnn {
namespace arm_common {

class LSTMCellImpl : public naive::LSTMCellImpl {
public:
    using naive::LSTMCellImpl::LSTMCellImpl;
    void exec(
            _megdnn_tensor_in input, _megdnn_tensor_in weight_ih,
            _megdnn_tensor_in bias_ih, _megdnn_tensor_in hx,
            _megdnn_tensor_in weight_hh, _megdnn_tensor_in bias_hh,
            _megdnn_tensor_in cx, _megdnn_tensor_out h_new, _megdnn_tensor_out c_new,
            _megdnn_tensor_out gates, _megdnn_workspace workspace) override;

    size_t get_workspace_in_bytes(
            const TensorLayout& input, const TensorLayout& weight_ih,
            const TensorLayout& bias_ih, const TensorLayout& hx,
            const TensorLayout& weight_hh, const TensorLayout& bias_hh,
            const TensorLayout& cx, const TensorLayout& h_new,
            const TensorLayout& c_new, const TensorLayout& gates) override;
};

}  // namespace arm_common
}  // namespace megdnn

// vim: syntax=cpp.doxygen
