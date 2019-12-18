#ifndef ETALON_UPDATER_H
#define ETALON_UPDATER_H

#include<iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <vector>
#include <QRect>


using namespace std;
using namespace cv;

class Etalon_updater
{
public:
    Etalon_updater();
    QRect search(Mat image, Mat debug);
    vector<cv::Point> coordinates;
private:
    int str;
    int max = 0;
    int x = 0;
    int y = 0;
    int xCoor = 0;
    int yCoor = 0;

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

};

#endif // ETALON_UPDATER_H
