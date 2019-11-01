#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // запилить норм функцию
    originalMat = imread("/home/liza/!QTProjects/technical_view/fox2.jpg", IMREAD_GRAYSCALE);
    cv::resize(originalMat, originalMat, Size(640, 480));
    originalPix = Mat2QPixmap(originalMat) ;
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

QPixmap Widget::Mat2QPixmap(cv::Mat const& src)
{
    cv::Mat temp = src.clone();
    return QPixmap::fromImage(QImage((uchar*) temp.data, temp.cols,
                                     temp.rows, temp.step, QImage::Format_Grayscale8));
}

cv::Mat Widget::QPixmap2Mat(QImage const& src)
{


    originalMat(cv::Rect(currentRect.x(),currentRect.y(),
                         currentRect.width(),currentRect.height())).copyTo(etalonMat);

//    Mat cropedImage = croppedRef(originalMat, currentRect);
//    QImage temp = src.copy();
//    cv::Mat res(temp.height(),temp.width(),CV_8UC3,(uchar*)temp.bits(),temp.bytesPerLine());
//    //    cvtColor(res, res,CV_BGR2RGB); // make convert colort to BGR !
    imshow("124", etalonMat);
    return etalonMat;
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
    etalonPix = originalPix.copy(currentRect);
    ui->etalon->setScaledContents(true);
    ui->etalon->setMaximumSize(QSize(ui->etalon->x(), ui->etalon->y()));
    ui->etalon->setPixmap(etalonPix);


   QPixmap2Mat(etalonPix.toImage());


}
