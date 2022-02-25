#pragma once

#include "megbrain_build_config.h"
#if MGB_JIT && MGB_JIT_MLIR

#include "megbrain/comp_node.h"
#include "megbrain/opr/param_defs.h"
#include "megdnn/basic_types.h"
#include "megdnn/opr_param_defs.h"

#include <mlir/IR/Dialect.h>
#include <mlir/IR/Function.h>
#include <mlir/IR/OpDefinition.h>
#include <mlir/Interfaces/SideEffectInterfaces.h>

namespace mgb {
namespace jit {

class MgbDialect : public ::mlir::Dialect {
public:
    explicit MgbDialect(::mlir::MLIRContext* ctx);

    //! We should register this function in dialect
    static llvm::StringRef getDialectNamespace() { return "mgb::jit"; }
};

}  // namespace jit
}  // namespace mgb

#define GET_OP_CLASSES
using namespace mlir;
#include "megbrain/jit/mlir/ir/mgb_dialect.h.inc"

#endif  // MGB_JIT && MGB_JIT_MLIR

// vim: syntax=cpp.doxygen
