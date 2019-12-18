#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QFileDialog>
#include <QMessageBox>
#include <Qt>

#include "criterion_function_evaluator.h"
#include "etalon_updater.h"

#include <vector>
#include <string.h>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/ml.h>
#include <opencv2/core/utility.hpp>
using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    int AAA = 1;
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    QPixmap Mat2QPixmap(Mat const& src);
    void createNewMatEtalon();
    void createNewQPixmapEtalon();
    vector<String> getImageFilenames();
    void loadImagesFromPath(vector<String> imgFilenames);
    void updateRoi();
    void updateEtalon(Mat previous_etalon, Mat current_etalon, Mat newEtalon);
QImage Mat2QImage(const cv::Mat_<double> &src);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void on_setEtalonHandle_clicked();
    void on_saveEtalon_clicked();
    void on_fileDialogButton_clicked();
    void on_startTracking_clicked();
    void on_stopTracking_clicked();
    void on_saveNoiseSettingsButton_clicked();
    void on_saveCryterySettingsButton_clicked();
    void on_setEtalonFromCoordinates_clicked();

private:
    Ui::Widget *ui;
    QPainter painter;
    QRect currentRect;
    QRect roiRect;
    QPixmap currentPix;
    QPixmap etalonPix;

    double blurValue;
    double noiseValue;
    QString noiseType;

    QString cryteryFunctionType;

    const int imageHeight = 640;
    const int imageWidth = 480;

    Mat etalonMat;
    Mat roiMat;
    Mat currentMat;

    vector<Mat> videoSequence;

    bool saveEtalon;
    bool isSetEtalonHandle;
    bool mousePressed;
    bool drawStarted;
    bool isStop;
    bool isSetEtalonFromCoordinates = false;
};

#endif // WIDGET_H
