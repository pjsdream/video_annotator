#ifndef KINECT_VIEWER_H
#define KINECT_VIEWER_H


#include <video_annotator/interface/viewer.h>
#include <video_annotator/kinect/kinect_device.h>


namespace video_annotator
{

class KinectViewerWidget : public InteractiveViewerWidget
{
    Q_OBJECT

public:

    KinectViewerWidget(QWidget* parent = 0);
    KinectViewerWidget(int fps, QWidget* parent = 0);

protected:

    virtual void initializeGL();
    virtual void resizeGL(int w, int h);
    virtual void paintGL();

private:

};

}


#endif // KINECT_VIEWER_H
