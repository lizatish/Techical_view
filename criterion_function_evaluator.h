#ifndef CRITERION_FUNCTION_H
#define CRITERION_FUNCTION_H

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
    Mat calculation_criterion(Mat src, Mat srccrop);
    Mat debug;
    int num;

private:
    // Разностная функция
    void func1(int I, int Ic);
    // Квадрат разности
    void func2(int I, int Ic);
    // Корреляционный критерий
    void func3(int I, int Ic);
    // Нормированная корреляция
    void func4(int I, int Ic);
    // Выбор критериальной функции
    void switch_f(int I, int Ic);
    // Функция вычисления значений пикселей
    void src_error(Mat src, Mat srccrop);
    int errorI = 0;
    uint64_t errorSum = 0;
    int coeff,coeff2;
    int d;
    int sumI, sumIc;

    Mat src, srccrop;
};

#endif // CRITERION_FUNCTION_H
