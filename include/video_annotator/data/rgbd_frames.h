#ifndef RGBD_FRAMES_H
#define RGBD_FRAMES_H


#include <string>
#include <vector>

#include <stdio.h>
#include <stdint.h>


namespace video_annotator
{

class RGBDFrames
{
public:

    RGBDFrames();

    bool load(const std::string& filename);
    void close();

    inline int numFrames() const
    {
        return num_frames_;
    }

    void getRGBD(int frame, std::vector<uint8_t>& rgb, std::vector<uint16_t>& depth);

private:

    int width_;
    int height_;
    double scale_;

    std::string filename_;
    FILE* rgbd_file_;
    int frame_offset_;
    int frame_size_;
    int num_frames_;
};

}


#endif // RGBD_FRAMES_H
