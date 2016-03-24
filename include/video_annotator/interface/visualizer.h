#ifndef VISUALIZER_H
#define VISUALIZER_H


#include <QOpenGLWidget>

#include <video_annotator/kinect/kinect_device.h>


namespace video_annotator
{

class KinectVideoWidget : public QOpenGLWidget
{
public:

    KinectVideoWidget(const KinectDevice* device, QWidget* parent = 0);

private:

    const KinectDevice* device_;
};

}


#endif // VISUALIZER_H
