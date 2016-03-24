#include <video_annotator/kinect/kinect_device.h>

#include <cmath>


namespace video_annotator
{

KinectDevice::KinectDevice(freenect_context *ctx, int index)
    : FreenectDevice(ctx, index)
    , m_buffer_depth(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes)
    , m_buffer_video(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2)
    , m_new_rgb_frame(false)
    , m_new_depth_frame(false)
{
    setDepthFormat(FREENECT_DEPTH_REGISTERED);
}

//~MyFreenectDevice(){}
// Do not call directly even in child
void KinectDevice::VideoCallback(void* rgb, uint32_t timestamp)
{
    Mutex::ScopedLock lock(m_rgb_mutex);

    uint8_t* rgb_cast = static_cast<uint8_t*>(rgb);
    std::copy(rgb_cast, rgb_cast + getVideoBufferSize(), m_buffer_video.begin());
    m_new_rgb_frame = true;
}

// Do not call directly even in child
void KinectDevice::DepthCallback(void* depth, uint32_t timestamp)
{
    Mutex::ScopedLock lock(m_depth_mutex);

    uint16_t* depth_cast = static_cast<uint16_t*>(depth);
    std::copy(depth_cast, depth_cast + getDepthBufferSize() / 2, m_buffer_depth.begin());
    m_new_depth_frame = true;
}

bool KinectDevice::getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth)
{
    Mutex::ScopedLock lock_rgb(m_rgb_mutex);
    Mutex::ScopedLock lock_depth(m_depth_mutex);

    if (!m_new_rgb_frame || !m_new_depth_frame)
        return false;

    rgb.swap(m_buffer_video);
    depth.swap(m_buffer_depth);

    m_new_rgb_frame = false;
    m_new_depth_frame = false;

    return true;
}

}
