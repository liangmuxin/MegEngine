#include "./opr_impl.h"
#include "./kern_wrapper.cuh"
#include "./special_kerns.cuh"

#include "src/cuda/utils.h"

namespace megdnn {
namespace cuda {

#define on_arity_dispatched_cb_dtype(_dt)                           \
    if (m_dst->layout.dtype == _dt()) {                             \
        using dtrait = DTypeTrait<_dt>;                             \
        using ctype = dtrait::ctype;                                \
        auto stream = cuda_stream(handle());                        \
        return ModeDispatcher<arity, dtrait::category, ctype>::run( \
                src, stream, m_param.mode, m_dst->ptr<ctype>());    \
    }

#define _cb_dispatch_mode(_m)                                                     \
    case Mode::_m:                                                                \
        do {                                                                      \
            using KernImpl = ElemwiseKern<                                        \
                    megcorePlatformCUDA, param_enumv::Elemwise::Mode::_m, ctype>; \
            using Wrapper = ElemArithKernWrapper<arity, KernImpl>;                \
            Wrapper wrapper;                                                      \
            wrapper.dst = static_cast<ctype*>(dst);                               \
            return run_elemwise<Wrapper, ctype, arity>(src, stream, wrapper);     \
        } while (0);

#define IMPL_MODE_DISPATCHER(_arity, _dtype_cat)                                    \
    template <typename ctype>                                                       \
    struct ElemwiseForwardImpl::ModeDispatcher<_arity, _dtype_cat, ctype> {         \
        static constexpr int arity = _arity;                                        \
        static void run(                                                            \
                const ElemwiseOpParamN<arity>& src, cudaStream_t stream, Mode mode, \
                void* dst) {                                                        \
            switch (mode) {                                                         \
                FOREACH(_cb_dispatch_mode)                                          \
                default:                                                            \
                    megdnn_throw("bad mode");                                       \
            }                                                                       \
        }                                                                           \
    }

#include "src/common/elemwise/opr_impl_body.inl"

template <typename ctype, bool c_is_scalar>
void ElemwiseForwardImpl::impl_fuse_mul_add3(const ElemwiseOpParamN<3>& param) {
    kern_fuse_mul_add3<c_is_scalar, ctype>(
            m_dst->ptr<ctype>(), param, cuda_stream(handle()));
}

template <typename ctype>
void ElemwiseForwardImpl::impl_fuse_mul_add4(const ElemwiseOpParamN<4>& param) {
    kern_fuse_mul_add4(m_dst->ptr<ctype>(), param, cuda_stream(handle()));
}

}  // namespace cuda
}  // namespace megdnn

// vim: syntax=cpp.doxygen
