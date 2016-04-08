#ifndef POINT_CLOUD_VIEWER_H
#define POINT_CLOUD_VIEWER_H


#include <video_annotator/interface/viewer.h>

#include <QTimer>


namespace video_annotator
{

class PointCloudViewerWidget : public Viewer3DWidget
{
    Q_OBJECT

public:

    PointCloudViewerWidget(QWidget* parent = 0);

private:
};

}


#endif // POINT_CLOUD_VIEWER_H
