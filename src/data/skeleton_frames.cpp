#include <video_annotator/data/skeleton_frames.h>


namespace video_annotator
{

SkeletonFrames::SkeletonFrames()
{
}

bool SkeletonFrames::load(const std::string &filename)
{
    file_skeleton_ = fopen(filename.c_str(), "rb");
    if (file_skeleton_ == NULL)
    {
        fprintf(stderr, "Error: cannot open file [%s]\n");
        fprintf(stderr, "Make sure that file exists and permission allowed\n");
        fflush(stderr);

        filename_ = "";
        return false;
    }

    filename_ = filename;

    int n;
    while (fscanf(file_skeleton_, "%d", &n) == 1)
    {
        std::vector<std::vector<double> > skeleton;
        for (int i=0; i<n; i++)
        {
            std::vector<double> p;
            for (int i=0; i<20 * 4; i++)
            {
                double v;
                fscanf(file_skeleton_, "%lf", &v);
                p.push_back(v);
            }
            skeleton.push_back(p);
        }
        skeletons_.push_back(skeleton);
    }

    fclose(file_skeleton_);

    return true;
}

void SkeletonFrames::getSkeleton(int frame, std::vector<std::vector<double> >& skeleton)
{
    skeleton = skeletons_[frame];
}

}
