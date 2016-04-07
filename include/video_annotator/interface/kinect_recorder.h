#ifndef KINECT_RECORDER_H
#define KINECT_RECORDER_H


#include <QMainWindow>
#include <QWidget>
#include <QLineEdit>

#include <video_annotator/interface/kinect_viewer.h>
#include <video_annotator/kinect/kinect_device.h>

#include <string>


namespace video_annotator
{

class RecordInterfaceWidget : public QWidget
{
    Q_OBJECT
public:

    RecordInterfaceWidget(QWidget* parent = 0);

signals:

    void filenameChanged(QString filename);
    void dialogOpened();
    void dialogClosed();
    void startRecord(std::string filename);
    void finishRecord();

private slots:

    void selectFilename(bool);
    void testToggled(bool checked);

private:

    QLineEdit* line_edit_;
};

class KinectRecorderWindow : public QMainWindow
{
    Q_OBJECT

public:

    KinectRecorderWindow(KinectDevice* device);
    KinectRecorderWindow(int fps, KinectDevice* device);
    ~KinectRecorderWindow();

private slots:

    void recordStarted(std::string filename);
    void recordFinished();

private:

    int fps_;
    KinectDevice* device_;

    KinectViewerWidget* widget_viewer_;
    RecordInterfaceWidget* widget_record_;
};

}


#endif // KINECT_RECORDER_H
