#include <megdnn/oprs.h>

#include "src/common/cv/helper.h"

namespace megdnn {
namespace aarch64 {

/**
 * \fn warp_perspective_cv
 * \brief Used if the format is NHWC, transfer from megcv
 */
void warp_perspective_cv_exec(
        _megdnn_tensor_in src, _megdnn_tensor_in trans, _megdnn_tensor_in mat_idx,
        _megdnn_tensor_in dst, float border_value,
        param::WarpPerspective::BorderMode border_mode,
        param::WarpPerspective::InterpolationMode imode, Handle* handle);

}  // namespace aarch64
}  // namespace megdnn

// vim: syntax=cpp.doxygen
