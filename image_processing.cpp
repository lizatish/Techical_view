#include "image_processing.h"

image_processing::image_processing()
{

}

Mat image_processing::addNoiseAndBlur(Mat src, QString noiseType, float noiseValue, float blurValue){
    if (noiseType == "Гауссовский"){
        image_noise = Gaussian_noise(src, noiseValue * 100);
    }
    else if (noiseType == "Импульсный"){
        image_noise = Saltpepper_noise(src, noiseValue * 15);
    }
    else if (noiseType == "Равномерный"){
        noiseValue = noiseValue/2 + 1;
        image_noise = uniform_noise(src, noiseValue);
    }
    else{
        image_noise = src.clone();
    }

    if(blurValue != 0){
        image_noise = Gaussian_blur(image_noise, blurValue);
    }
    return image_noise;
}

Mat image_processing::Gaussian_noise(Mat image, int x){
    cv::Mat noise(image.size(),image.type());

    float m = (10,12,34);
    float sigma = (1,5,x*100);
    cv::randn(noise, m, sigma); //mean and variance
    image += noise;
    return image;
}

Mat image_processing::uniform_noise(Mat img, int n){
    Mat saltpepper_noise = Mat::zeros(img.rows, img.cols,CV_8U);
    randu(saltpepper_noise,0,255);

    Mat white = saltpepper_noise > 225/n;
    Mat saltpepper_img = img.clone();
    saltpepper_img.setTo(255,white);

    return saltpepper_img;
}

Mat image_processing::Saltpepper_noise(Mat image, int n){
    cv::Mat uniform_noise = cv::Mat::zeros (image.rows, image.cols, n);
    cv::randu(uniform_noise, 0, 255);

    Mat black = uniform_noise < 30*n;
    Mat white = uniform_noise > 225;

    Mat uniform_img = image.clone();
    uniform_img.setTo(255,white);
    uniform_img.setTo(0,black);

    return uniform_img;
}

Mat image_processing::Gaussian_blur(Mat image, int n){
    GaussianBlur(image, image, Size(n, n), 0);
    return image;
}
