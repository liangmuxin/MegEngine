/***************************************************************************************************
 * Copyright (c) 2017-2019, NVIDIA CORPORATION.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 *permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright notice, this
 *list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice, this
 *list of conditions and the following disclaimer in the documentation and/or other
 *materials provided with the distribution.
 *     * Neither the name of the NVIDIA CORPORATION nor the names of its contributors
 *may be used to endorse or promote products derived from this software without specific
 *prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 *EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 *SHALL NVIDIA CORPORATION BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 *HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *OR TOR (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **************************************************************************************************/
#pragma once
#include "src/cuda/convolution_helper/global_memory_visitor/global_memory_visitor_common.cuh"
#include "src/cuda/convolution_helper/layout.cuh"

namespace megdnn {
namespace cuda {
namespace convolution {

template <bool check_bounds, typename TileCount_, typename Layout>
struct Global2ShareMemVisitor_CIxWOxN;

DEF_GLOBAL_MEMORY_VISITOR(Global2ShareMemVisitor_CIxWOxN, Layout<Format::CHWN4>)
using RegBlockConfig = typename TileCount::RegBlockConfig;
using ThreadConfig = typename TileCount::ThreadConfig;
int sw;
int stride;
int remain;
int img_stride;
int img_start;
int img_end;

const int tidx = threadIdx.x;
const int tidy = threadIdx.y;
const int tid = tidy * ThreadConfig::nr_thread_x + tidx;
const int gl_load_y = tid / TileCount::load_x;
const int gl_load_x = tid - gl_load_y * TileCount::load_x;

copy_t reg[TileCount::reg_h][TileCount::img_cache][TileCount::reg_w];

__device__ __forceinline__ void init_stride(Layout<Format::CHWN4> layout) {
    stride = layout.channel_stride / TileCount::ldg_load_width;
    img_stride = layout.width_stride / TileCount::ldg_load_width;
}

__device__ __forceinline__ void first_copy() {
#pragma unroll
    for (int i = 0; i < TileCount::reg_h; ++i) {
        int h_idx = gl_load_y + i * TileCount::load_y;
        if (TileCount::check_bounds_h && h_idx >= TileCount::smem_h)
            continue;
#pragma unroll
        for (int j = 0; j < TileCount::img_cache; ++j) {
            int jstride = j * sw;
#pragma unroll
            for (int k = 0; k < TileCount::reg_w; ++k) {
                int w_idx = gl_load_x + k * TileCount::load_x;
                int batch = w_idx * load_width;
                if (TileCount::check_bounds_w && w_idx >= TileCount::smem_load_x)
                    continue;
                if (check_bounds) {
                    copy_t val = make_zero<copy_t>();
                    if (jstride >= img_start && jstride < img_end && batch < remain) {
                        val = g_ptr[h_idx * stride + jstride * img_stride + w_idx];
                    }
                    *(sh_ptr_as_copy_t(h_idx, j, batch)) = val;
                } else {
                    copy_t val = make_zero<copy_t>();
                    if (jstride >= img_start && jstride < img_end) {
                        val = g_ptr[h_idx * stride + jstride * img_stride + w_idx];
                    }
                    *(sh_ptr_as_copy_t(h_idx, j, batch)) = val;
                }
            }
        }
    }
}

__device__ __forceinline__ void copy() {
#pragma unroll
    for (int i = 0; i < TileCount::reg_h; ++i) {
        int h_idx = gl_load_y + i * TileCount::load_y;
        if (TileCount::check_bounds_h && h_idx >= TileCount::smem_h)
            continue;
#pragma unroll
        for (int j = 0; j < TileCount::img_cache; ++j) {
            int jstride = j * sw;
#pragma unroll
            for (int k = 0; k < TileCount::reg_w; ++k) {
                int w_idx = gl_load_x + k * TileCount::load_x;
                int batch = w_idx * load_width;
                if (TileCount::check_bounds_w && w_idx >= TileCount::smem_load_x)
                    continue;
                if (check_bounds) {
                    copy_t val = make_zero<copy_t>();
                    if (jstride >= img_start && jstride < img_end && batch < remain) {
                        val = g_ptr[h_idx * stride + jstride * img_stride + w_idx];
                    }
                    reg[i][j][k] = val;
                } else {
                    copy_t val = make_zero<copy_t>();
                    if (jstride >= img_start && jstride < img_end) {
                        val = g_ptr[h_idx * stride + jstride * img_stride + w_idx];
                    }
                    reg[i][j][k] = val;
                }
            }
        }
    }
}

__device__ __forceinline__ void commit() {
#pragma unroll
    for (int i = 0; i < TileCount::reg_h; ++i) {
        int h_idx = gl_load_y + i * TileCount::load_y;
        if (TileCount::check_bounds_h && h_idx >= TileCount::smem_h)
            continue;
#pragma unroll
        for (int j = 0; j < TileCount::img_cache; ++j) {
#pragma unroll
            for (int k = 0; k < TileCount::reg_w; ++k) {
                int w_idx = gl_load_x + k * TileCount::load_x;
                if (TileCount::check_bounds_w && w_idx >= TileCount::smem_load_x)
                    continue;
                *(sh_ptr_as_copy_t(h_idx, j, w_idx * load_width)) = reg[i][j][k];
            }
        }
    }
}

__device__ __forceinline__ int32_t* sh_ptr(int z, int y, int x) {
    return &smem[(z * TileCount::img_cache + y) * TileCount::smem_stride + x];
}

__device__ __forceinline__ copy_t* sh_ptr_as_copy_t(int z, int y, int x) {
    return reinterpret_cast<copy_t*>(sh_ptr(z, y, x));
}

__device__ __forceinline__ void move_forward() {
    g_ptr += RegBlockConfig::reg_k_packed * stride;
}

__device__ __forceinline__ void set_range(const int start, const int end) {
    img_start = start, img_end = end;
}
};

}  // namespace convolution
}  // namespace cuda
}  // namespace megdnn

// vim: ft=cpp syntax=cuda.doxygen foldmethod=marker foldmarker=f{{{,f}}}
