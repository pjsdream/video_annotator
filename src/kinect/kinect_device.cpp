#include <video_annotator/kinect/kinect_device.h>

#include <cmath>


namespace video_annotator
{

KinectDevice::KinectDevice(freenect_context *ctx, int index)
    : FreenectDevice(ctx, index)
    , buffer_depth_(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes)
    , buffer_video_(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2)
    , new_rgb_frame_(false)
    , new_depth_frame_(false)
{
    setDepthFormat(FREENECT_DEPTH_REGISTERED);
}

//~MyFreenectDevice(){}
// Do not call directly even in child
void KinectDevice::VideoCallback(void* rgb, uint32_t timestamp)
{
    Mutex::ScopedLock lock(rgb_mutex_);

    uint8_t* rgb_cast = static_cast<uint8_t*>(rgb);
    std::copy(rgb_cast, rgb_cast + getVideoBufferSize(), buffer_video_.begin());
    new_rgb_frame_ = true;
}

// Do not call directly even in child
void KinectDevice::DepthCallback(void* depth, uint32_t timestamp)
{
    Mutex::ScopedLock lock(depth_mutex_);

    uint16_t* depth_cast = static_cast<uint16_t*>(depth);
    std::copy(depth_cast, depth_cast + getDepthBufferSize() / 2, buffer_depth_.begin());
    new_depth_frame_ = true;
}

bool KinectDevice::getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth)
{
    Mutex::ScopedLock lock_rgb(rgb_mutex_);
    Mutex::ScopedLock lock_depth(depth_mutex_);

    if (!new_rgb_frame_ || !new_depth_frame_)
        return false;

    rgb.swap(buffer_video_);
    depth.swap(buffer_depth_);

    new_rgb_frame_ = false;
    new_depth_frame_ = false;

    return true;
}

}
