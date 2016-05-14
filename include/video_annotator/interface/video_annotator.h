#ifndef VIDEO_ANNOTATOR_H
#define VIDEO_ANNOTATOR_H


#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include <QIntValidator>

#include <video_annotator/data/rgbd_frames.h>
#include <video_annotator/data/skeleton_frames.h>
#include <video_annotator/interface/viewer.h>
#include <video_annotator/interface/point_cloud_viewer.h>

#include <string>


namespace video_annotator
{

class TimeBar : public QWidget
{
    Q_OBJECT

public:

    TimeBar(QWidget* parent);

signals:

    void frameChanged(int frame);

protected slots:

    void updateNumFrames(int num_frames);
    void updateValue(int value);
    void updateValue(QString value);

private:

    QSlider* slider_;
    QLineEdit* frame_;
    QIntValidator* frame_range_;
    QLabel* label_;
};


class VideoAnnotatorWindow : public QMainWindow
{
    Q_OBJECT

public:

    enum ViewMode
    {
        VIEW_RGB = 0,
        VIEW_DEPTH,
        VIEW_POINTCLOUD,
    };

public:

    VideoAnnotatorWindow();

    void load(const std::string& filename);

signals:

    void numFramesChanged(int num_frames);
    void frameChanged(int frame);

protected slots:

    void openOpenDialog();
    void updateFrame(int frame);
    void viewRGB();
    void viewDepth();

private:

    std::string rgbd_filename_;
    std::string skeleton_filename_;
    std::string annotation_filename_;

    ViewMode view_mode_;
    int current_frame_;

    RGBDFrames frames_;
    SkeletonFrames skeleton_frames_;
    ImageViewerWidget* widget_viewer_;
    PointCloudViewerWidget* widget_pointcloud_viewer_;
    TimeBar* time_bar_;
};

}


#endif // VIDEO_ANNOTATOR_H
