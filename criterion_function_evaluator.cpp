#include "criterion_function_evaluator.h"

/// НЕОБХОДИМО ИЗМЕНЯТЬ РАЗМЕРНОСТИ ИСХОДНОГО ИЗОБРАЖЕНИЯ ВРУЧНУЮ
///
/// MatrixI[X][Y]
/// debug(Y,X,CV_8UC1)
///


Criterion_function_evaluator::Criterion_function_evaluator()
{

}
Mat Criterion_function_evaluator::calculation_criterion(Mat src, Mat srccrop){

    cout << "src: " << src.rows << " " << src.cols << endl;
    cout << "srccrop: " << srccrop.rows << " " << srccrop.cols << endl;

    // Входим в функцию расчета значений пикселей
    return src_error(src, srccrop);
}


// Разностная функция
void Criterion_function_evaluator::func1(int I, int Ic){
    errorI = abs(I-Ic);
    errorSum = errorI + errorSum;
}

// Квадрат разности
void Criterion_function_evaluator::func2(int I, int Ic){
    errorI = abs(I-Ic)*abs(I-Ic);
    errorSum = errorI + errorSum;
}

// Корреляционный критерий
void Criterion_function_evaluator::func3(int I, int Ic){
    errorI = abs(I*Ic);
    errorSum = errorI + errorSum;
}

// Нормированная корреляция
void Criterion_function_evaluator::func4(int I, int Ic){
    sumI = sumI+I;
    sumIc = sumIc + Ic;
    errorSum = sumI + sumIc;
}

// Выбор критериальной функции
void Criterion_function_evaluator::switch_f(int I, int Ic){
    //////////////// num = 1; ЭТО НУЖНО ПОТОМ ИЗМЕНИТЬ
    num = 1;
    switch(num)
    {
    case 1:
        func1(I,Ic);
        coeff = 255;
        coeff2 = 255;
        break;
    case 2:
        func2(I,Ic);
        coeff = 255;
        coeff2 = 3;
        break;
    case 3:
        func3(I,Ic);
        coeff = 255;
        coeff2 = 4;
        break;
    case 4:
        func4(I,Ic);
        coeff = 2;
        coeff2 = 1;
        break;
    default:
        break;

    }
}

// Функция вычисления значений пикселей
Mat Criterion_function_evaluator::src_error(Mat src, Mat srccrop)
{
    Mat debug = src.clone();
    float I, Ic;
    // Проходим по всем строкам, эталон сдвигается на deltax после каждого попиксельного прохода по эталону.
    for (int deltax = 0; deltax < src.rows; deltax++)
    {
        // Проходим по всем столбцам, deltay - сдвиг по оси y
        for (int deltay = 0; deltay < src.cols; deltay++)
        {
            // Проходим по строкам эталона
            for (int i =0; i < srccrop.rows; i++)
            {
                // Проходим по столбцам эталона
                for( int j = 0; j < srccrop.cols; j++)
                {
                    // Вычисление значений пикселей эталона и исходного одноканального изображения
                    Ic = (int)srccrop.at<uchar>(i,j);
                    I = (int)src.at<uchar>(i+deltax, j+deltay);

                    // Выбираем критериальную функцию
                    switch_f(I,Ic);
                }
            }

            // Записываем значения в матрицу размерностью такой же, как исходное изображение
            //            MatrixI[deltax][deltay] = errorSum;

            // Вычисляем значения пикселей путем нормализации
            d = errorSum * coeff2 /(srccrop.rows*srccrop.cols*coeff);
            cout << deltax << " " << deltay << " | " << errorSum << " " << d << endl;

            // Записываем значения пикселей в матрицу изображения
            debug.at<uchar>(Point(deltay, deltax)) = d;

            // Для нормированной корреляции используются переменные sumI, sumIc
            // Служат для накопления ошибки
            // После каждой итерации необходимо обнулять
            if (num == 4)
            {
                sumI = 0;
                sumIc = 0;
            }
            // Обнуляем переменные, накапливающие сумму ошибок
            errorSum = 0;
            errorI = 0;
        }
    }
    return debug;

}
