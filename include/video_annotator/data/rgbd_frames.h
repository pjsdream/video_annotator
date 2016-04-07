#ifndef RGBD_FRAMES_H
#define RGBD_FRAMES_H


#include <string>
#include <vector>

#include <stdio.h>


namespace video_annotator
{

class RGBDFrames
{
public:

    bool load(const std::string& filename);
    void close();

    void getRGBD(std::vector<uint8_t>& rgb, std::vector<uint16_t> depth);

private:

    int width_;
    int height_;
    double transform_[16];

    std::string filename_;
    FILE* rgbd_file_;
    int frame_offset_;
};

}


#endif // RGBD_FRAMES_H
