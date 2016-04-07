#include <video_annotator/interface/kinect_recorder.h>

#include <QGridLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QDockWidget>


namespace video_annotator
{

RecordInterfaceWidget::RecordInterfaceWidget(QWidget* parent)
    : QWidget(parent)
{
    setWindowTitle("Record");

    QPushButton* save_button = new QPushButton(tr("Save As..."), this);
    connect(save_button, SIGNAL(clicked(bool)), this, SIGNAL(dialogOpened()));
    connect(save_button, SIGNAL(clicked(bool)), this, SLOT(selectFilename(bool)));

    QPushButton* record_button = new QPushButton(tr("Record"), this);
    record_button->setCheckable(true);
    connect(record_button, SIGNAL(toggled(bool)), this, SLOT(testToggled(bool)));

    line_edit_ = new QLineEdit(this);
    connect(this, SIGNAL(filenameChanged(QString)), line_edit_, SLOT(setText(QString)));

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(save_button, 0, 0);
    layout->addWidget(line_edit_, 0, 1);
    layout->addWidget(record_button, 1, 0);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);
    setLayout(layout);
}

void RecordInterfaceWidget::selectFilename(bool)
{
    QString q_filename = QFileDialog::getSaveFileName(this, tr("Save As..."), line_edit_->text());
    emit dialogClosed();
    emit filenameChanged(q_filename);
}

void RecordInterfaceWidget::testToggled(bool checked)
{
    if (checked)
        emit startRecord(line_edit_->text().toStdString());
    else
        emit finishRecord();
}


KinectRecorderWindow::KinectRecorderWindow(KinectDevice* device)
    : KinectRecorderWindow(30, device)
{
}

KinectRecorderWindow::KinectRecorderWindow(int fps, KinectDevice* device)
    : QMainWindow()
    , fps_(fps)
    , device_(device)
    , widget_viewer_(new KinectViewerWidget(fps, device))
{
    widget_viewer_->setMinimumSize(1280, 480);
    widget_viewer_->setMaximumSize(1280, 480);
    setCentralWidget(widget_viewer_);

    QDockWidget *dock = new QDockWidget(this);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    widget_record_ = new RecordInterfaceWidget(dock);
    connect(widget_record_, SIGNAL(dialogOpened()), widget_viewer_, SLOT(pause()));
    connect(widget_record_, SIGNAL(dialogClosed()), widget_viewer_, SLOT(resume()));
    connect(widget_record_, SIGNAL(destroyed(QObject*)), this, SLOT(close()));
    connect(widget_record_, SIGNAL(startRecord(std::string)), this, SLOT(recordStarted(std::string)));
    connect(widget_record_, SIGNAL(finishRecord()), this, SLOT(recordFinished()));
    widget_record_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    dock->setWidget(widget_record_);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    setWindowTitle("Kinect viewer");
    move(100, 100);
    setAttribute(Qt::WA_DeleteOnClose);
}

KinectRecorderWindow::~KinectRecorderWindow()
{
    delete widget_record_;
}

void KinectRecorderWindow::recordStarted(std::string filename)
{
    try
    {
        device_->startRecord(filename);
    }
    catch (...)
    {
    }
}

void KinectRecorderWindow::recordFinished()
{
    device_->finishRecord();
}

}
