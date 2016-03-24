#ifndef KINECT_DEVICE_H
#define KINECT_DEVICE_H


#include <libfreenect.hpp>

#include <video_annotator/util/mutex.h>

#include <vector>


namespace video_annotator
{

/** 640 x 480 resolution Kinect
 */
class KinectDevice : public Freenect::FreenectDevice
{
public:

    KinectDevice(freenect_context *ctx, int index);

    /** to convert from image space (i,j,d) to 3d space (x,y,z),
     * (from https://openkinect.org/wiki/Imaging_Information)
     *
     * x = (i - w / 2) * (z + minDistance) * scaleFactor
     * y = (j - h / 2) * (z + minDistance) * scaleFactor
     * z = z
     * Where
     * minDistance = -10
     * scaleFactor = .0021.
     * These values were found by hand.
     */

    // Do not call directly even in child
    void VideoCallback(void* rgb, uint32_t timestamp);

    // Do not call directly even in child
    void DepthCallback(void* depth, uint32_t timestamp);

    // depth is in mm
    bool getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth);

private:

    std::vector<uint8_t> m_buffer_video;
    std::vector<uint16_t> m_buffer_depth;
    Mutex m_rgb_mutex;
    Mutex m_depth_mutex;
    bool m_new_rgb_frame;
    bool m_new_depth_frame;
};

}


#endif // KINECT_DEVICE_H
