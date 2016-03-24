#ifndef KINECT_DEVICE_H
#define KINECT_DEVICE_H


#include <libfreenect.hpp>

#include <video_annotator/util/mutex.h>

#include <vector>


namespace video_annotator
{

/** 640 x 480 resolution Kinect device
 */
class KinectDevice : public Freenect::FreenectDevice
{
public:

    KinectDevice(freenect_context *ctx, int index);

    /* to convert from image space (i,j,d) to 3d space (x,y,z),
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

    /* 640*480 resolution, r(1) g(1) b(1) d(2)
     * 1.46MB/frame
     * 15fps => 22.0MB/s
     * 30fps => 43.9MB/s
     * Typical SSD data transfer rate = 100~600MB/s
     */

    // Do not call directly even in child
    void VideoCallback(void* rgb, uint32_t timestamp);

    // Do not call directly even in child
    void DepthCallback(void* depth, uint32_t timestamp);

    // depth is in mm
    bool getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth);

private:

    std::vector<uint8_t> buffer_video_;
    std::vector<uint16_t> buffer_depth_;
    Mutex rgb_mutex_;
    Mutex depth_mutex_;
    bool new_rgb_frame_;
    bool new_depth_frame_;
};

}


#endif // KINECT_DEVICE_H
