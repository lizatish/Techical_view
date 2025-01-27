#-------------------------------------------------
#
# Project created by QtCreator 2019-10-30T21:30:30
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cursach_tz
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        widget.cpp \
        image_widget.cpp \
        main.cpp \
    criterion_function_evaluator.cpp \
    image_processing.cpp

HEADERS += \
    widget.h \
    image_widget.h \
    criterion_function_evaluator.h \
    image_processing.h


FORMS += \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

CONFIG += link_pkgconfig
PKGCONFIG += opencv

#LIBS += `pkg-config --cflags --libs opencv`
TARGET = opencv
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_core \
 -lopencv_videoio \
 -lopencv_imgcodecs \
 -lopencv_imgproc \
 -lopencv_calib3d \
 -lopencv_video \
 -lopencv_features2d \
 -lopencv_ml \
 -lopencv_highgui \
 -lopencv_objdetect \
