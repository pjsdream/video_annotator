#include <video_annotator/kinect/kinect_device.h>

#include <cmath>

#include <video_annotator/util/time.h>


namespace video_annotator
{

#ifdef _WIN32

int KinectDevice::depthToRgbMap[width * height * 2];

KinectDevice::KinectDevice()
    : buffer_video_(640 * 480 * 3)
    , buffer_depth_(640 * 480)
    , new_rgb_frame_(false)
    , new_depth_frame_(false)
    , file_rgbd_(NULL)
    , record_thread_(this)
{
    setVideoFormat(KINECT_VIDEO_RGB);
    initialized_ = initKinect();
}

void KinectDevice::setVideoFormat(KinectVideoFormat format)
{
    video_format_ = format;
}

KinectDevice::KinectVideoFormat KinectDevice::getVideoFormat()
{
    return video_format_;
}

bool KinectDevice::initKinect()
{
    // Get a working kinect sensor
    int num_sensors;
    if (NuiGetSensorCount(&num_sensors) < 0 || num_sensors < 1) return false;
    if (NuiCreateSensorByIndex(0, &sensor_) < 0) return false;

    HRESULT result;

    // Initialize sensor
    result = sensor_->NuiInitialize(NUI_INITIALIZE_FLAG_USES_DEPTH | NUI_INITIALIZE_FLAG_USES_COLOR);
    if (result != S_OK)
    {
        fprintf(stderr, "Error: Kinect is not initialized after NuiInitialize().\n");
        fflush(stderr);
        return false;
    }

    result = sensor_->NuiImageStreamOpen(
                NUI_IMAGE_TYPE_DEPTH,                     // Depth camera or rgb camera?
                NUI_IMAGE_RESOLUTION_640x480,             // Image resolution
                0,      // Image stream flags, e.g. near mode
                2,      // Number of frames to buffer
                NULL,   // Event handle
                &depth_stream_);
    if (result != S_OK)
    {
        reportImageStreamOpenError(result);
        return false;
    }

    result = sensor_->NuiImageStreamOpen(
                NUI_IMAGE_TYPE_COLOR,                     // Depth camera or rgb camera?
                NUI_IMAGE_RESOLUTION_640x480,             // Image resolution
                0,      // Image stream flags, e.g. near mode
                2,      // Number of frames to buffer
                NULL,   // Event handle
                &rgb_stream_);
    if (result != S_OK)
    {
        reportImageStreamOpenError(result);
        return false;
    }

    return sensor_;
}

void KinectDevice::reportImageStreamOpenError(HRESULT result)
{
    switch (result)
    {
    case E_FAIL:
        fprintf(stderr, "Error: An unspecified error occurred.\n");
        fflush(stderr);
        break;

    case E_INVALIDARG:
        fprintf(stderr, "Error: The value of the dwFlags parameter is NULL.\n");
        fflush(stderr);
        break;

    case E_NUI_DEVICE_NOT_READY:
        fprintf(stderr, "Error: Kinect has not been initialized.\n");
        fflush(stderr);
        break;

    case E_OUTOFMEMORY:
        fprintf(stderr, "Error: The allocation failed.\n");
        fflush(stderr);
        break;

    case E_POINTER:
        fprintf(stderr, "Error: The hNextFrameEvent parameter is an invalid handle.\n");
        fflush(stderr);
        break;

    default:
        fprintf(stderr, "Error: Unknown error code of NuiImageStreamOpenError.\n");
        fflush(stderr);
        break;
    }
}

bool KinectDevice::getRGB(uint8_t* rgb)
{
    NUI_IMAGE_FRAME image_frame;
    NUI_LOCKED_RECT locked_rect;

    if (!initialized_ || sensor_->NuiImageStreamGetNextFrame(rgb_stream_, 0, &image_frame) < 0)
        return false;

    INuiFrameTexture* texture = image_frame.pFrameTexture;
    texture->LockRect(0, &locked_rect, NULL, 0);

    // rgb
    if (locked_rect.Pitch != 0)
    {
        const BYTE* start = (const BYTE*) locked_rect.pBits;
        for (int j = 0; j < height; ++j) {
            for (int i = 0; i < width; ++i) {
                *rgb++ = start[2];
                *rgb++ = start[1];
                *rgb++ = start[0];
                start+=4;
            }
        }
    }

    texture->UnlockRect(0);
    sensor_->NuiImageStreamReleaseFrame(rgb_stream_, &image_frame);

    return locked_rect.Pitch != 0;
}

bool KinectDevice::getDepth(uint16_t* depth)
{
    NUI_IMAGE_FRAME image_frame;
    NUI_LOCKED_RECT locked_rect;

    if (!initialized_ || sensor_->NuiImageStreamGetNextFrame(depth_stream_, 0, &image_frame) < 0)
        return false;

    INuiFrameTexture* texture = image_frame.pFrameTexture;
    texture->LockRect(0, &locked_rect, NULL, 0);

    // depth
    if (locked_rect.Pitch != 0)
    {
        const USHORT* curr = (const USHORT*) locked_rect.pBits;
        for (int j = 0; j < height; ++j)
        {
            for (int i = 0; i < width; ++i)
            {
                // Get depth of pixel in millimeters
                *depth++ = NuiDepthPixelToDepth(*curr++);
            }
        }
    }

    texture->UnlockRect(0);
    sensor_->NuiImageStreamReleaseFrame(depth_stream_, &image_frame);

    return locked_rect.Pitch != 0;
}

bool KinectDevice::getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth)
{
    getRGB(&buffer_video_[0]);
    getDepth(&buffer_depth_[0]);

    // Copy the vector. Can be slower than swap
    rgb_mutex_.lock();
    rgb = buffer_video_;
    rgb_mutex_.unlock();

    depth_mutex_.lock();
    depth = buffer_depth_;
    depth_mutex_.unlock();

    return true;
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

bool KinectDevice::getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth)
{
    // Copy the vector. Can be slower than swap
    rgb_mutex_.lock();
    rgb = buffer_video_;
    rgb_mutex_.unlock();

    depth_mutex_.lock();
    depth = buffer_depth_;
    depth_mutex_.unlock();

    return true;
}
#endif

void KinectDevice::startRecord(const std::string &filename)
{
    if (file_rgbd_)
        finishRecord();

    file_rgbd_ = fopen(filename.c_str(), "wb");
    if (file_rgbd_ == NULL)
    {
        throw std::string("Unable to access file \"") + filename + std::string("\".\n") +
                std::string("Permission denied?");
    }
    recording_ = true;

    int sizes[2] = {640, 480};
    fwrite((void*)sizes, sizeof(int), 2, file_rgbd_);
    double scale = 0.0021;
    fwrite(&scale, sizeof(double), 1, file_rgbd_);

#ifdef _WIN32
    record_thread_.start();
#else
    pthread_create(&record_thread_, NULL, &staticRecord, this);
#endif
}

void KinectDevice::finishRecord()
{
    recording_ = false;

#ifdef _WIN32
    record_thread_.wait();
#else
    pthread_join(record_thread_, NULL);
#endif

    fclose(file_rgbd_);
    file_rgbd_ = NULL;
}

#ifdef _WIN32
KinectDevice::KinectDeviceRecordThread::KinectDeviceRecordThread(KinectDevice *device)
    : device_(device)
{
}
void KinectDevice::KinectDeviceRecordThread::run()
{
    device_->record();
}
#else
void* KinectDevice::staticRecord(void* object)
{
    (static_cast<KinectDevice*>(object))->record();
    return 0;
}
#endif

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
