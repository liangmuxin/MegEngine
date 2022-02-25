#include "src/common/utils.h"

#include <cstdlib>
#include "./default_device_context.hpp"

using namespace megcore;
using namespace megcore::cpu;
using namespace megdnn;

DefaultDeviceContext::DefaultDeviceContext(int device_id, unsigned int flags)
        : DeviceContext(megcorePlatformCPU, device_id, flags) {
    megdnn_assert(device_id == -1);
}

DefaultDeviceContext::~DefaultDeviceContext() noexcept = default;

size_t DefaultDeviceContext::mem_alignment_in_bytes() const noexcept {
    return 1;
}

void DefaultDeviceContext::activate() noexcept {}

void* DefaultDeviceContext::malloc(size_t size_in_bytes) {
    return new uint8_t[size_in_bytes];
}

void DefaultDeviceContext::free(void* ptr) {
    delete[] static_cast<uint8_t*>(ptr);
}

// vim: syntax=cpp.doxygen
