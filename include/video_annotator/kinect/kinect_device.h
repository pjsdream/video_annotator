#ifndef KINECT_DEVICE_H
#define KINECT_DEVICE_H


#ifdef _WIN32
    #include <stdint.h>

    #include <Windows.h>
    #include <Ole2.h>
    #include <NuiApi.h>
    #include <NuiImageCamera.h>
    #include <NuiSkeleton.h>
    #include <NuiSensor.h>

    // QThread for recording thread
    #include <QThread>
#else
    #include <libfreenect.hpp>
#endif

#include <video_annotator/util/mutex.h>

#include <vector>
#include <string>
#include <stdio.h>


namespace video_annotator
{

#ifdef _WIN32

    /** 640 x 480 resolution Kinect device
    */
    class KinectDevice
    {
    private:

        class KinectDeviceRecordThread : public QThread
        {
        public:
            KinectDeviceRecordThread(KinectDevice* device);

        private:
            void run();

            KinectDevice* device_;
        };

    public:

        enum KinectVideoFormat
        {
            KINECT_VIDEO_RGB = 0,
            KINECT_VIDEO_YUV_RGB,
        };

        static const int width = 640;
        static const int height = 480;

        KinectDevice();

        void setVideoFormat(KinectVideoFormat format);
        KinectVideoFormat getVideoFormat();

        bool getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth);

        void startRecord(const std::string& filename);
        void finishRecord();

    private:

        void reportImageStreamOpenError(HRESULT result);

        bool initKinect();

        bool getRGB(uint8_t* rgb);
        bool getDepth(uint16_t* depth);

        void record();

        // Kinect
        INuiSensor* sensor_;
        HANDLE rgb_stream_;
        HANDLE depth_stream_;
        bool initialized_;

        // Kinect buffer
        static int depthToRgbMap[width * height * 2];

        std::vector<uint8_t> buffer_video_;
        std::vector<uint16_t> buffer_depth_;
        mutable Mutex rgb_mutex_;
        mutable Mutex depth_mutex_;
        bool new_rgb_frame_;
        bool new_depth_frame_;

        FILE* file_rgbd_;
        bool recording_;
        KinectDeviceRecordThread record_thread_;

        KinectVideoFormat video_format_;
    };

#else

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
        virtual void VideoCallback(void* rgb, uint32_t timestamp);

        // Do not call directly even in child
        virtual void DepthCallback(void* depth, uint32_t timestamp);

        // depth is in mm
        bool getRGBD(std::vector<uint8_t> &rgb, std::vector<uint16_t> &depth);

        void startRecord(const std::string& filename);
        void finishRecord();

    private:

        static void* staticRecord(void* object);
        void record();

        std::vector<uint8_t> buffer_video_;
        std::vector<uint16_t> buffer_depth_;
        mutable Mutex rgb_mutex_;
        mutable Mutex depth_mutex_;
        bool new_rgb_frame_;
        bool new_depth_frame_;

        FILE* file_rgbd_;
        pthread_t record_thread_;
        bool recording_;
    };

#endif

}


#endif // KINECT_DEVICE_H
