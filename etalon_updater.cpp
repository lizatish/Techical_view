#include "etalon_updater.h"

Etalon_updater::Etalon_updater()
{

}
vector<int> Etalon_updater::search(Mat image, Mat debug){

    int width = debug.rows;
    int height = debug.cols;

    threshold( debug, thr, 50,255,THRESH_BINARY );

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
    cv::Rect rect(xCoor, yCoor, width, height);
    // and its top left corner...
    cv::Point pt1(xCoor, yCoor);
    // and its bottom right corner.
    cv::Point pt2(xCoor + width, yCoor + height);
    // These two calls...
    cv::rectangle(image, pt1, pt2, cv::Scalar(0, 0, 255));
    // essentially do the same thing
    cv::rectangle(image, rect, cv::Scalar(0, 0, 255));

    coordinates.push_back(xCoor);
    coordinates.push_back(yCoor);

    return coordinates;
}
