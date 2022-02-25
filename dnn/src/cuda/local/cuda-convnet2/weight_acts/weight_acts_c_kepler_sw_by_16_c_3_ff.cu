#include "wet_act_c_kepler_sw.cuh"

namespace megdnn {
namespace cuda {

WET_ACT_C_KEPLER_SW_HEAD<16, 16, 2, 16, 1, 32, 3, false, false>(C_KEP_SW_PARAM);
// WET_ACT_C_KEPLER_SW_HEAD< 16, 16, 2, 16, 1, 32, 3, false, true > (C_KEP_SW_PARAM);
// WET_ACT_C_KEPLER_SW_HEAD< 16, 16, 2, 16, 1, 32, 3, true, false > (C_KEP_SW_PARAM);
// WET_ACT_C_KEPLER_SW_HEAD< 16, 16, 2, 16, 1, 32, 3, true, true > (C_KEP_SW_PARAM);

}  // namespace cuda
}  // namespace megdnn
