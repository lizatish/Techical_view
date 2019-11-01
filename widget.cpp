#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // запилить норм функцию
    Mat img = imread("/home/liza/!QTProjects/technical_view/fox2.jpg", IMREAD_GRAYSCALE);
    cv::resize(img, img, Size(640, 480));
    QImage qim2 = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_Grayscale8);
    originalPix = QPixmap::fromImage(qim2);
    currentPix = originalPix.copy();

    mousePressed = true;
    drawStarted = false;
    saveEtalon = false;
    changeEtalon = false;
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent* event){
    mousePressed = true;
    currentRect.setTopLeft(event->pos());
    currentRect.setBottomRight(event->pos());
}

void Widget::mouseReleaseEvent(QMouseEvent *event){
    mousePressed = false;
    update();
}

void Widget::mouseMoveEvent(QMouseEvent* event){
    if(event->type() == QEvent::MouseMove and changeEtalon){
        currentPix = originalPix.copy();
        currentRect.setBottomRight(event->pos());
    }
    update();
}

void Widget::paintEvent(QPaintEvent *event){

    painter.begin(this);
    if(mousePressed){
        painter.drawPixmap(0, 0, currentPix);
        painter.drawRect(currentRect);
        drawStarted = true;
    }
    else if (drawStarted){
        QPainter tempPainter(&currentPix);
        tempPainter.drawRect(currentRect);
        painter.drawPixmap(0, 0, currentPix);
    }
    painter.end();
}

void Widget::on_changeEtalon_clicked()
{
    changeEtalon = true;
}

void Widget::on_saveEtalon_clicked()
{
    saveEtalon = true;
    changeEtalon = false;
    // вырезаем эталон
    etalonPix = currentPix.copy(currentRect);
    ui->etalon->setScaledContents(true);
    ui->etalon->setMaximumSize(QSize(110,110));
    ui->etalon->setPixmap(etalonPix);

}
QImage Widget::Mat2QImage(cv::Mat const& src)
{
    cv::Mat temp(src.cols, src.rows, src.type()); // make the same cv::Mat
    //    cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
    QImage dest= QImage((uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_Grayscale8);
    return dest;
}

cv::Mat Widget::QImage2Mat(QImage const& src)
{
    QImage temp = src.copy();
    cv::Mat res(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
    //    cvtColor(res, res,CV_BGR2RGB); // make convert colort to BGR !
    return res;
}
