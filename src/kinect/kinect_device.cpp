#include <video_annotator/kinect/kinect_device.h>

#include <cmath>

#include <video_annotator/util/time.h>


namespace video_annotator
{

#ifdef _WIN32 || _WIN64
KinectDevice::KinectDevice()
    : buffer_video_(640 * 480 * 3)
    , buffer_depth_(640 * 480)
    , new_rgb_frame_(false)
    , new_depth_frame_(false)
    , file_rgbd_(NULL)
{
    setVideoFormat(KINECT_VIDEO_RGB);
}

void KinectDevice::setVideoFormat(KinectVideoFormat format)
{
    video_format_ = format;
}

KinectDevice::KinectVideoFormat KinectDevice::getVideoFormat()
{
    return video_format_;
}
#else
KinectDevice::KinectDevice(freenect_context *ctx, int index)
    : FreenectDevice(ctx, index)
    , buffer_video_(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes)
    , buffer_depth_(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2)
    , new_rgb_frame_(false)
    , new_depth_frame_(false)
    , file_rgbd_(NULL)
{
    setDepthFormat(FREENECT_DEPTH_REGISTERED);
}

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
#endif

bool KinectDevice::getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth) const
{
    if (!new_rgb_frame_ || !new_depth_frame_)
        return false;

    // Copy the vector. Can be slower than swap
    rgb_mutex_.lock();
    rgb = buffer_video_;
    rgb_mutex_.unlock();

    depth_mutex_.lock();
    depth = buffer_depth_;
    depth_mutex_.unlock();

    return true;
}

void KinectDevice::startRecord(const std::string &filename)
{
    if (file_rgbd_ != NULL)
    {
        finishRecord();
    }

    file_rgbd_ = fopen(filename.c_str(), "wb");
    if (file_rgbd_ == NULL)
    {
        throw std::string("Unable to access file \"") + filename + std::string("\".\n") +
                std::string("Permission denied?");
    }

    int sizes[2] = {640, 480};
    fwrite((void*)sizes, sizeof(int), 2, file_rgbd_);
    double scale = 0.0021;
    fwrite(&scale, sizeof(double), 1, file_rgbd_);

    recording_ = true;
#ifdef _WIN32 || _WIN64
#else
    pthread_create(&record_thread_, NULL, &staticRecord, this);
#endif
}

void KinectDevice::finishRecord()
{
    recording_ = false;

#ifdef _WIN32 || _WIN64
#else
    pthread_join(record_thread_, NULL);
#endif

    fclose(file_rgbd_);
    file_rgbd_ = NULL;
}

void* KinectDevice::staticRecord(void* object)
{
    (static_cast<KinectDevice*>(object))->record();
    return 0;
}

void KinectDevice::record()
{
    Rate rate(30);

    std::vector<uint8_t> rgb;
    std::vector<uint16_t> depth;

    while (recording_)
    {
        printf("recording %lf...", time());

        if (getRGBD(rgb, depth))
        {
            rgb_mutex_.lock();
            rgb = buffer_video_;
            rgb_mutex_.unlock();

            depth_mutex_.lock();
            depth = buffer_depth_;
            depth_mutex_.unlock();

            fwrite((void*)&rgb[0], sizeof(rgb[0]), rgb.size(), file_rgbd_);
            fwrite((void*)&depth[0], sizeof(depth[0]), depth.size(), file_rgbd_);
        }
        else
        {
            printf("No data available!");
        }

        printf("\n");
        fflush(stdout);
        rate.sleep();
    }
}

}
