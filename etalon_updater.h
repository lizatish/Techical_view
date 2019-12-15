#ifndef ETALON_UPDATER_H
#define ETALON_UPDATER_H

#include<iostream>
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <vector>

using namespace std;
using namespace cv;

class Etalon_updater
{
public:
    Etalon_updater();
    vector<int> search(Mat image, Mat debug);
    vector<int> coordinates;
private:
    int str;
    int max = 0;
    int x = 0;
    int y = 0;
    float xCoor = 0;
    float yCoor = 0;
    Mat thr;

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

};

#endif // ETALON_UPDATER_H
