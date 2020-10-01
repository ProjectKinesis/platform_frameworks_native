/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_GUI_SURFACE_CONTROL_H
#define ANDROID_GUI_SURFACE_CONTROL_H

#include <stdint.h>
#include <sys/types.h>

#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/threads.h>

#include <ui/FrameStats.h>
#include <ui/PixelFormat.h>
#include <ui/Region.h>

#include <gui/ISurfaceComposerClient.h>
#include <math/vec3.h>

namespace android {

// ---------------------------------------------------------------------------

class IGraphicBufferProducer;
class Surface;
class SurfaceComposerClient;

// ---------------------------------------------------------------------------

class SurfaceControl : public RefBase
{
public:
    static status_t readFromParcel(const Parcel& parcel, sp<SurfaceControl>* outSurfaceControl);
    status_t writeToParcel(Parcel& parcel);

    static bool isValid(const sp<SurfaceControl>& surface) {
        return (surface != nullptr) && surface->isValid();
    }

    bool isValid() {
        return mHandle!=nullptr && mClient!=nullptr;
    }

    static bool isSameSurface(
            const sp<SurfaceControl>& lhs, const sp<SurfaceControl>& rhs);

    // Reparent off-screen and release. This is invoked by the destructor.
    void destroy();

    // disconnect any api that's connected
    void        disconnect();

    static status_t writeSurfaceToParcel(
            const sp<SurfaceControl>& control, Parcel* parcel);

    sp<Surface> getSurface() const;
    sp<Surface> createSurface() const;
    sp<IBinder> getHandle() const;

    sp<IGraphicBufferProducer> getIGraphicBufferProducer() const;

    status_t clearLayerFrameStats() const;
    status_t getLayerFrameStats(FrameStats* outStats) const;

    sp<SurfaceComposerClient> getClient() const;

    uint32_t getTransformHint() const;

    void setTransformHint(uint32_t hint);

    explicit SurfaceControl(const sp<SurfaceControl>& other);

    SurfaceControl(const sp<SurfaceComposerClient>& client, const sp<IBinder>& handle,
                   const sp<IGraphicBufferProducer>& gbp, uint32_t transformHint = 0);

private:
    // can't be copied
    SurfaceControl& operator = (SurfaceControl& rhs);
    SurfaceControl(const SurfaceControl& rhs);

    friend class SurfaceComposerClient;
    friend class Surface;

    ~SurfaceControl();

    sp<Surface> generateSurfaceLocked() const;
    status_t validate() const;

    sp<SurfaceComposerClient>   mClient;
    sp<IBinder>                 mHandle;
    sp<IGraphicBufferProducer>  mGraphicBufferProducer;
    mutable Mutex               mLock;
    mutable sp<Surface>         mSurfaceData;
    uint32_t mTransformHint;
};

}; // namespace android

#endif // ANDROID_GUI_SURFACE_CONTROL_H
