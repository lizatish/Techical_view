#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->image, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_current_pos()));
    originalPix.load("/home/liza/!QTProjects/technical_view/fox2.jpg");
    mPix = originalPix.copy();
    //        mPix = QPixmap(400,400);

    //        mPix.fill(Qt::white);


    mousePressed = false;
    drawStarted = false;


}

Widget::~Widget()
{
    delete ui;
}
void Widget::Mouse_current_pos()
{
    qDebug("%d %d", ui->image->x, ui->image->y );
}

void Widget::mousePressEvent(QMouseEvent* event){
    //Mouse is pressed for the first time
    mousePressed = true;

    //    //set the initial line points, both are same
    mRect.setTopLeft(event->pos());
    mRect.setBottomRight(event->pos());
    qDebug("IOOO" );

}
void Widget::mouseReleaseEvent(QMouseEvent *event){

    //When mouse is released update for the one last time
    mousePressed = false;
    update();
}
void Widget::mouseMoveEvent(QMouseEvent* event){
    if(event->type() == QEvent::MouseMove){
        mRect.setBottomRight(event->pos());
    }
    update();
}
void Widget::paintEvent(QPaintEvent *event){

    painter.begin(this);

    if(mousePressed){
        mPix = originalPix.copy();
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
