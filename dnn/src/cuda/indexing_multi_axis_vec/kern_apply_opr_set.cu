#include "src/common/indexing_multi_axis_vec_kdef.h"
#define KERN_APPLY_OPR_OPR ::megdnn::indexing_multi_axis_vec_kdef::OprSet
#include "./kern_apply_opr_impl.cuinl"

// vim: ft=cuda syntax=cpp.doxygen
