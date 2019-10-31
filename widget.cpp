#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //    connect(ui->image, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_current_pos()));
    originalPix.load("/home/liza/!QTProjects/technical_view/fox2.jpg");
    mPix = originalPix.copy();

    mousePressed = true;
    drawStarted = false;
    saveEtalon = false;
    changeEtalon = false;

}

Widget::~Widget()
{
    delete ui;
}
void Widget::Mouse_current_pos()
{
    //    qDebug("%d %d", ui->image->x, ui->image->y );
}

void Widget::mousePressEvent(QMouseEvent* event){

    mousePressed = true;

    mRect.setTopLeft(event->pos());
    mRect.setBottomRight(event->pos());
    qDebug("IOOO" );

}
void Widget::mouseReleaseEvent(QMouseEvent *event){
    mousePressed = false;
    update();
}
void Widget::mouseMoveEvent(QMouseEvent* event){
    if(event->type() == QEvent::MouseMove and changeEtalon){
        mPix = originalPix.copy();
        mRect.setBottomRight(event->pos());
    }
    update();
}
void Widget::paintEvent(QPaintEvent *event){

    painter.begin(this);
    if(mousePressed){
        painter.drawPixmap(0, 0, mPix);
        painter.drawRect(mRect);
        drawStarted = true;
    }
    else if (drawStarted){
        QPainter tempPainter(&mPix);
        tempPainter.drawRect(mRect);
        painter.drawPixmap(0, 0, mPix);
    }
    painter.end();

}
void Widget::Mouse_Pressed()
{

}

void Widget::Mouse_Left()
{

}


void Widget::on_changeEtalon_clicked()
{
    changeEtalon = true;
}

void Widget::on_saveEtalon_clicked()
{
    saveEtalon = true;
    changeEtalon = false;
}
