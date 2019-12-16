#include "etalon_updater.h"

Etalon_updater::Etalon_updater()
{

}
vector<QRect> Etalon_updater::search(Mat etalonImage, Mat debugImage){

    int width = etalonImage.rows;
    int height = etalonImage.cols;

    threshold( debugImage, thr, 50,255,THRESH_BINARY );

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


    int nx = width/4; //число на которое увеличиваем ROI относительно эталона
    int ny = height/4;

    int width_roi = width+2*nx;
    int height_roi = height+2*ny;

    int nx_down = xCoor-nx + width_roi;
    int ny_down = yCoor-ny + height_roi; //координаты нижнего угла

    cout <<"Ширина рои " <<width_roi <<" Длина рои " << height_roi<<endl;
    cout <<"Координаты нижнего угла Х " <<nx_down <<" У " << ny_down<<endl;

    if (nx_down  > debugImage.rows){
        nx_down  = debugImage.rows - xCoor + nx;
        width_roi = nx_down;
    }
    if (ny_down > debugImage.cols){
        ny_down = debugImage.cols - yCoor + ny;
        height_roi = ny_down;
    }
    //cout << nx_down <<"    " << ny_down<<endl;
    cout << "Измененная длина и ширина рои "<< width_roi <<"    " << height_roi<<endl;

    QRect rect_roi(xCoor-nx,yCoor-ny,width_roi,height_roi);


    cout <<"Координаты нужного пикселя х = "<< xCoor<<" "<<"y = "<<yCoor<<endl;
    QRect new_etalon(xCoor, yCoor, height, width);

    vector<QRect> data;
    data.push_back(new_etalon);
    data.push_back(rect_roi);
    return data;
}
