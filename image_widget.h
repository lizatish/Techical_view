#ifndef IMAGE_WIDGET_H
#define IMAGE_WIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QEvent>

class Image_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Image_widget(QWidget *parent = nullptr);
//    void mouseMoveEvent(QMouseEvent *ev);
//    void leaveEvent(QEvent *ev);
//    void mousePressEvent(QMouseEvent *ev);

    int x, y;
signals:
    void Mouse_Pressed();
    void Mouse_Pos();
    void Mouse_Left();

public slots:
};

#endif // IMAGE_WIDGET_H
