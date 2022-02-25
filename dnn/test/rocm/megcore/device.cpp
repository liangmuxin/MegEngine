#include "hcc_detail/hcc_defs_prologue.h"
#include "megcore.h"

#include "./fixture.h"
#include "hip_header.h"
#include "test/common/utils.h"
#include "test/rocm/utils.h"

TEST_F(MegcoreROCM, DEVICE) {
    for (int id = -1; id < std::min(nr_devices(), 2); ++id) {
        megcoreDeviceHandle_t handle;
        megcoreCreateDeviceHandle(&handle, megcorePlatformROCM, id, 0);

        int deviceID;
        megcoreGetDeviceID(handle, &deviceID);
        ASSERT_EQ(id, deviceID);

        megcorePlatform_t platform;
        megcoreGetPlatform(handle, &platform);
        ASSERT_EQ(megcorePlatformROCM, platform);

        unsigned int flags;
        megcoreGetDeviceFlags(handle, &flags);
        ASSERT_EQ(0u, flags);

        size_t memAlignmentInBytes;
        megcoreGetMemAlignment(handle, &memAlignmentInBytes);

        megcoreActivate(handle);

        void* ptr;
        megcoreMalloc(handle, &ptr, 256);
        megcoreFree(handle, ptr);

        megcoreDestroyDeviceHandle(handle);
    }
}

TEST_F(MegcoreROCM, ERROR_MSG) {
#if MEGDNN_ENABLE_EXCEPTIONS
    megcoreDeviceHandle_t handle;
    ASSERT_THROW(
            megcoreCreateDeviceHandle(&handle, megcorePlatformROCM, nr_devices(), 0),
            megdnn::test::MegDNNError);
    hipGetLastError();
    hip_check(hipGetLastError());
#endif
}

// vim: syntax=cpp.doxygen
