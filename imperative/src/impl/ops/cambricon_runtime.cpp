#include "../op_trait.h"
#include "megbrain/imperative/ops/autogen.h"

#if MGB_CAMBRICON
#include "megbrain/cambricon/cambricon_runtime_opr.h"
namespace mgb::imperative {

namespace {
namespace cambricon_runtime {

auto apply_on_var_node(const OpDef& def, const VarNodeArray& inputs) {
    auto&& op = static_cast<const CambriconRuntime&>(def);
    SymbolVarArray symbol_var_inputs(inputs.begin(), inputs.end());
    OperatorNodeConfig config{op.make_name()};
    return opr::CambriconRuntimeOpr::make(
            op.buf.c_str(), op.buf_size, op.symbol, symbol_var_inputs,
            op.tensor_dim_mutable, config);
}
OP_TRAIT_REG(CambriconRuntime, CambriconRuntime)
        .apply_on_var_node(apply_on_var_node)
        .fallback();
}  // namespace cambricon_runtime
}  // namespace

}  // namespace mgb::imperative
#endif