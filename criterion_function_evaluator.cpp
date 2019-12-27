#include "criterion_function_evaluator.h"

/// НЕОБХОДИМО ИЗМЕНЯТЬ РАЗМЕРНОСТИ ИСХОДНОГО ИЗОБРАЖЕНИЯ ВРУЧНУЮ
///
/// MatrixI[X][Y]

///


Criterion_function_evaluator::Criterion_function_evaluator()
{

}
Mat Criterion_function_evaluator::calculateCriterionFunction(Mat src, Mat srccrop, int match_method){
    // Входим в функцию расчета значений пикселей
    std::cout << "Вычисление критериальной функции" << std::endl;

    //Проверяем,непустыелиизображения
    if(src.empty()||srccrop.empty())
    {
        cout << "Исходное изображение отсутствует" << endl;
    }

    //Вычисляемразмеррезультирующегоизображения
    int result_cols=src.cols-srccrop.cols+1;
    int result_rows=src.rows-srccrop.rows+1;

    //Создаѐм объект для хранения результата
    debug.create( result_rows, result_cols, CV_32FC1 );
    //Сопоставляем шаблон
    matchTemplate( src, srccrop, debug, match_method);
    //Нормализуем полученный результат
    normalize( debug, debug, 0, 1, NORM_MINMAX, -1, Mat() );

    return debug;
}

QRect Criterion_function_evaluator::getEtalonCoordinates(Mat debug, int match_method, QRect roi, Mat etalon){
    double minValue = 0, maxValue = 0;
    Point minCoordinates, maxCoordinates, etalonCoordinates;
    minMaxLoc( debug, &minValue, &maxValue, &minCoordinates, &maxCoordinates, Mat() );

    //Выбираем точку локации в зависимости от использованного метода (минимум или максимум)
    if( match_method == TM_SQDIFF or match_method == TM_SQDIFF_NORMED ) {
        etalonCoordinates = minCoordinates;
    }
    else {
        etalonCoordinates = maxCoordinates;
    }

    return QRect(etalonCoordinates.x + roi.x(),  etalonCoordinates.y + roi.y(), etalon.cols, etalon.rows);
}

