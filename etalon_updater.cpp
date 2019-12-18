#include "etalon_updater.h"

Etalon_updater::Etalon_updater()
{

}
QRect Etalon_updater::search(Mat etalonImage, Mat debugImage){

    int width = etalonImage.rows;
    int height = etalonImage.cols;

    Mat thr;
    threshold( debugImage, thr, 30,255,THRESH_BINARY );

    Mat canny_output = thr.clone();


    blur( thr, thr, Size(3,3) );
    //    // detect edges using canny
    Canny( thr, canny_output, 100, 200, 3 );

    // find contours
    findContours( canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

    // get the moments
    vector<Moments> mu(contours.size());
    for( int i = 0; i<contours.size(); i++ ){
        mu[i] = moments( contours[i], false );
    }

    // get the centroid of figures.
    vector<Point2f> mc(contours.size());

    for( int i = 0; i<contours.size(); i++){
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        xCoor = mc[i].x;
        yCoor = mc[i].y;
    }

    if (xCoor >= etalonImage.rows){
        xCoor = etalonImage.rows;
    }
    else if (xCoor <= 0){
        xCoor = 0;
    }
    if (yCoor >= etalonImage.cols){
        yCoor = etalonImage.cols;
    }
    else if (xCoor <= 0){
        yCoor = 0;
    }
    QRect new_etalon(xCoor, yCoor, height, width);
    return  new_etalon;
}
