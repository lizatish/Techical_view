#ifndef CRITERION_FUNCTION_H
#define CRITERION_FUNCTION_H
#include <QRect>
#include<iostream>
#include"opencv2/imgproc.hpp"
#include"opencv2/highgui.hpp"
using namespace std;
using namespace cv;

class Criterion_function_evaluator
{
public:
    Criterion_function_evaluator();
    int** MatrixI;
    Mat calculateCriterionFunction(Mat src, Mat srccrop, int match_method);
    QRect getEtalonCoordinates(Mat debug, int match_method, QRect roi, Mat etalon);
    int num;
    Mat debug;

private:
    Mat src, srccrop;
};

#endif // CRITERION_FUNCTION_H
