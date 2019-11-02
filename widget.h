#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QImage>
#include <QFileDialog>
#include <QMessageBox>

#include <vector>
#include <string.h>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv/ml.h>
#include <opencv2/core/utility.hpp>
#include <glob.h>
using namespace cv;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
    bool mousePressed;
    bool drawStarted;


    QPixmap Mat2QPixmap(Mat const& src);
    void createNewMatEtalon();
    void createNewQPixmapEtalon();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void on_changeEtalon_clicked();
    void on_saveEtalon_clicked();
    void on_fileDialogButton_clicked();

private:
    Ui::Widget *ui;
    QPainter painter;
    QRect currentRect;
    QPixmap currentPix;
    QPixmap originalPix;
    QPixmap etalonPix;

    Mat etalonMat;
    Mat originalMat;

    bool saveEtalon;
    bool changeEtalon;


};

#endif // WIDGET_H
