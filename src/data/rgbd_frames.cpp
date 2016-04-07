#include <video_annotator/data/rgbd_frames.h>


namespace video_annotator
{

bool RGBDFrames::load(const std::string &filename)
{
    if (rgbd_file_ != NULL)
        close();

    rgbd_file_ = fopen(filename.c_str(), "rb");
    if (rgbd_file_ == NULL)
    {
        fprintf(stderr, "Error: cannot open file [%s]\n");
        fprintf(stderr, "Make sure that file exists and permission allowed\n");
        fflush(stderr);
        filename_ = "";
        return 0;
    }

    filename_ = filename;
    fread(&width_, sizeof(int), 1, rgbd_file_);
    fread(&height_, sizeof(int), 1, rgbd_file_);
    fread(&transform_, sizeof(double), 16, rgbd_file_);
    frame_offset_ = ftell(rgbd_file_);

    return true;
}

void RGBDFrames::getRGBD(std::vector<uint8_t>& rgb, std::vector<uint16_t> depth)
{
    rgb.resize(width_ * height_ * 3);
    depth.resize(width_ * height_);
    fread(&rgb[0], sizeof(uint8_t), width_ * height_ * 3, rgbd_file_);
    fread(&depth[0], sizeof(uint16_t), width_ * height_, rgbd_file_);
}

void RGBDFrames::close()
{
    if (rgbd_file_ != NULL)
        fclose(rgbd_file_);
}

}
