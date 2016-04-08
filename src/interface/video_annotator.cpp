#include <video_annotator/interface/video_annotator.h>

#include <QGridLayout>
#include <QDockWidget>
#include <QMenuBar>
#include <QFileDialog>


namespace video_annotator
{

// TimeBar
TimeBar::TimeBar(QWidget* parent)
    : QWidget(parent)
{
    // slider
    slider_ = new QSlider(Qt::Horizontal, this);
    slider_->setDisabled(true);
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(updateValue(int)));

    // label
    label_ = new QLabel("frame", this);

    // frame number text box
    frame_ = new QLineEdit(this);
    frame_->setAlignment(Qt::AlignCenter);
    frame_->setDisabled(true);
    frame_range_ = new QIntValidator(0, 1, frame_);
    frame_->setValidator(frame_range_);
    connect(frame_, SIGNAL(textChanged(QString)), this, SLOT(updateValue(QString)));

    // layout
    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(label_, 0, 0, Qt::AlignCenter);
    layout->addWidget(frame_, 1, 0);
    layout->addWidget(slider_, 1, 1);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);
    setLayout(layout);
}

void TimeBar::updateNumFrames(int num_frames)
{
    frame_range_->setTop(num_frames - 1);

    frame_->setEnabled(true);
    slider_->setEnabled(true);
    slider_->setMaximum(num_frames - 1);
    updateValue(slider_->value());
}

void TimeBar::updateValue(QString value)
{
    slider_->setValue(value.toInt());
    emit frameChanged(value.toInt());
}

void TimeBar::updateValue(int value)
{
    frame_->setText( QString::number(value) );
    emit frameChanged(value);
}


// VideoAnnotatorWindow
VideoAnnotatorWindow::VideoAnnotatorWindow()
    : QMainWindow()
{
    current_frame_ = 0;
    view_mode_ = VIEW_RGB;

    connect(this, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));

    // viewer widget
    widget_viewer_ = new ImageViewerWidget(this);
    setCentralWidget(widget_viewer_);

    // time bar
    QDockWidget *dock = new QDockWidget(this);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    time_bar_ = new TimeBar(this);
    connect(time_bar_, SIGNAL(frameChanged(int)), this, SLOT(updateFrame(int)));
    connect(this, SIGNAL(numFramesChanged(int)), time_bar_, SLOT(updateNumFrames(int)));
    dock->setWidget(time_bar_);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    // menu
    QAction* action_open = new QAction(tr("Open"), this);
    connect(action_open, SIGNAL(triggered(bool)), this, SLOT(openOpenDialog()));

    QMenu* menu_file = menuBar()->addMenu(tr("&File"));
    menu_file->addAction(action_open);

    QAction* action_view_rgb = new QAction(tr("RGB"), this);
    connect(action_view_rgb, SIGNAL(triggered(bool)), this, SLOT(viewRGB()));
    QAction* action_view_depth = new QAction(tr("depth"), this);
    connect(action_view_depth, SIGNAL(triggered(bool)), this, SLOT(viewDepth()));

    QMenu* menu_view = menuBar()->addMenu(tr("&View"));
    menu_view->addAction(action_view_rgb);
    menu_view->addAction(action_view_depth);
}

void VideoAnnotatorWindow::openOpenDialog()
{
    const QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Bin files (*.bin)"));
    load(filename.toStdString());
}

void VideoAnnotatorWindow::load(const std::string& filename)
{
    rgbd_filename_ = filename;
    annotation_filename_ = rgbd_filename_.substr(0, rgbd_filename_.length() - 4) + "_annotation.txt";

    frames_.load(rgbd_filename_);
    emit numFramesChanged(frames_.numFrames());
    updateFrame(0);
}

void VideoAnnotatorWindow::updateFrame(int frame)
{
    current_frame_ = frame;

    std::vector<uint8_t> rgb;
    std::vector<uint16_t> depth;
    frames_.getRGBD(frame, rgb, depth);

    switch (view_mode_)
    {
    case VIEW_RGB:
        widget_viewer_->updateRGBImage(rgb);
        break;

    case VIEW_DEPTH:
        widget_viewer_->updateDepthImage(depth);
        break;
    }
}

void VideoAnnotatorWindow::viewRGB()
{
    view_mode_ = VIEW_RGB;
    updateFrame(current_frame_);
}

void VideoAnnotatorWindow::viewDepth()
{
    view_mode_ = VIEW_DEPTH;
    updateFrame(current_frame_);
}

}
