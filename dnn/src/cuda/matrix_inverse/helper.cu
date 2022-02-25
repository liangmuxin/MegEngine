#include "./helper.cuh"
#include "src/cuda/error_info.cuh"

using namespace megdnn;
using namespace cuda;
using namespace matrix_inverse;

namespace {

__global__ void kern_check_error(
        const int* src_info, uint32_t n, megcore::AsyncErrorInfo* dst_info,
        void* tracker) {
    uint32_t i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n && src_info[i]) {
        set_async_error_info(
                dst_info, tracker,
                "The U is exactly singular and the inversion "
                "failed on %d-th input matrix (U(%d, %d) = 0)",
                i, src_info[i], src_info[i]);
    }
}

}  // anonymous namespace

void matrix_inverse::check_error(
        const int* src_info, uint32_t n, megcore::AsyncErrorInfo* dst_info,
        void* tracker, cudaStream_t stream) {
    if (!dst_info) {
        return;
    }
    uint32_t threads = NR_THREADS;
    uint32_t blocks = DIVUP(n, threads);
    kern_check_error<<<blocks, threads, 0, stream>>>(src_info, n, dst_info, tracker);
    after_kernel_launch();
}

// vim: syntax=cpp.doxygen
