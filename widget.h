#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>

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


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void Mouse_current_pos();
    void Mouse_Pressed();
    void Mouse_Left();

private:
    Ui::Widget *ui;
    QPainter painter;
    QRect mRect;
    QPixmap mPix;
    QPixmap originalPix;


};

#endif // WIDGET_H
