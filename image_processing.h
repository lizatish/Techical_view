#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H
#include <iostream>
#include <opencv2/opencv.hpp>

#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <string>
#include <QString>

using namespace cv;
using namespace std;

class image_processing
{
public:
    image_processing();
    Mat src;
    Mat image_noise;
    Mat addNoiseAndBlur(Mat src, QString choice_noise, float n, float degree_blur);
private:
    bool flag_noise = false;
    Mat Gaussian_noise(Mat image, int x);
    Mat Saltpepper_noise(Mat img, int n);
    Mat uniform_noise(Mat image, int n);
    Mat Gaussian_blur(Mat image, int n);
    void open();
    //выбор шума: 1 - Гауссовский, 2 - импульсный
//    int choice_noise = 0;
//    //гауссовский шум очень сильно зависит от 3 параметра в sigma,
//    //предлагаю его менять от 1 до 100 c шагом 20
//    //но внутри умножать на 100, чтобы разница была большой
//    int noise_gaussian_sigma3 = 0;
//    //импульсный шум предлагаю умножать на коэффициент, тк там и черный и белый параметр
//    //от 1 до 15 с шагом 1 можно сделать, при 15 ыообще почти ничего не видно
//    int n = 0;

//    int choice_blur;
//    int degree_blur;
};

#endif // IMAGE_PROCESSING_H
