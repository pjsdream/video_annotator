#ifndef SKELETON_FRAMES_H
#define SKELETON_FRAMES_H


#include <string>
#include <vector>

#include <stdio.h>
#include <stdint.h>


namespace video_annotator
{

class SkeletonFrames
{
public:

    SkeletonFrames();

    bool load(const std::string& filename);

    inline int numFrames() const
    {
        return skeletons_.size();
    }

    void getSkeleton(int frame, std::vector<std::vector<double> >& skeleton);

private:

    std::string filename_;
    FILE* file_skeleton_;
    std::vector<std::vector<std::vector<double> > > skeletons_;
};

}


#endif // RGBD_FRAMES_H
