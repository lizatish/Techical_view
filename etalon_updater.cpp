#include "etalon_updater.h"

Etalon_updater::Etalon_updater()
{

}
QRect Etalon_updater::search(Mat etalonImage, Mat debugImmage){

    int width = etalonImage.rows;
    int height = etalonImage.cols;

    threshold( debugImmage, thr, 50,255,THRESH_BINARY );

    // detect edges using canny
    Canny( thr, canny_output, 50, 150, 3 );

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

    cout <<"Координаты нужного пикселя х = "<< xCoor<<" "<<"y = "<<yCoor<<endl;
    QRect new_etalon(xCoor, yCoor, width, height);
    return new_etalon;
}
