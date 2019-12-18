#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    blurValue = 0;
    noiseValue = 0;
    noiseType = "";

    mousePressed = true;
    drawStarted = false;
    saveEtalon = false;
    isSetEtalonHandle = false;
    isStop = false;
    isSetEtalonFromCoordinates = false;
}

Widget::~Widget()
{
    delete ui;
}

QPixmap Widget::Mat2QPixmap(const cv::Mat& src)
{
    return QPixmap::fromImage(QImage((unsigned char*) src.data, src.cols, src.rows, QImage::Format_Grayscale8));

    //    cv::Mat temp = src.clone();
    //    QPixmap p = QPixmap::fromImage(Mat2QImage(src));
    //    p = p.scaledToWidth(src.cols);
    //    return p;

}

void Widget::createNewMatEtalon()
{
    currentMat(cv::Rect(currentRect.x(),currentRect.y(),
                        currentRect.width(),currentRect.height())).copyTo(etalonMat);
}

void Widget::createNewQPixmapEtalon()
{
    etalonPix = currentPix.copy(currentRect);
    ui->etalon->setScaledContents(true);
    ui->etalon->setMaximumSize(QSize(110, 110));
    ui->etalon->setPixmap(etalonPix);
}

void Widget::mousePressEvent(QMouseEvent* event){
    mousePressed = true;
    currentRect.setTopLeft(event->pos());
    currentRect.setBottomRight(event->pos());
}

void Widget::mouseReleaseEvent(QMouseEvent *event){
    mousePressed = false;
    update();
}

void Widget::mouseMoveEvent(QMouseEvent* event){
    if(event->type() == QEvent::MouseMove and isSetEtalonHandle
            and not isSetEtalonFromCoordinates){
        currentPix = Mat2QPixmap(currentMat);

        QPoint rectCoorfinates;
        if ((event->x() >= imageHeight - 2) and (event->y() >= imageWidth - 2)){
            rectCoorfinates.setX(imageHeight - 2);
            rectCoorfinates.setY(imageWidth - 2);
        }
        else if (event->x() >= imageHeight - 2){
            rectCoorfinates.setX(imageHeight - 2);
            rectCoorfinates.setY(event->y());
        }
        else if (event->y() >= imageWidth - 2){
            rectCoorfinates.setX(event->x());
            rectCoorfinates.setY(imageWidth - 2);
        }
        else{
            rectCoorfinates.setX(event->x());
            rectCoorfinates.setY(event->y());
        }

        currentRect.setBottomRight(rectCoorfinates);
        update();
    }
}

void Widget::paintEvent(QPaintEvent *event){

    painter.begin(this);
    if(mousePressed){
        painter.drawPixmap(0, 0, currentPix);
        if(currentRect.width() > 3 and currentRect.height() > 3){
            painter.setPen(QColor(255, 247, 28, 255));
            cout << 2 << endl;
            painter.drawRect(currentRect);
            createNewQPixmapEtalon();
            createNewMatEtalon();
            if (not isSetEtalonFromCoordinates){
                ui->xEtalonValue->setValue(currentRect.x());
                ui->yEtalonValue->setValue(currentRect.y());
                ui->widthEtalonValue->setValue(currentRect.width());
                ui->heightEtalonValue->setValue(currentRect.height());
            }
        }
        drawStarted = true;
    }
    else if (drawStarted){
        QPainter tempPainter(&currentPix);
        if(currentRect.width() > 3 and currentRect.height() > 3){
            //            tempPainter.setBrush(QColor(244,1,1, 127));
            tempPainter.setPen(QColor(255, 247, 28, 255));
            tempPainter.drawRect(currentRect);
            // Меняем эталоны
            if (isSetEtalonHandle or isSetEtalonHandle){
                createNewQPixmapEtalon();
                createNewMatEtalon();
                cout << 1 << endl;

                if (not isSetEtalonFromCoordinates){
                    ui->xEtalonValue->setValue(currentRect.x());
                    ui->yEtalonValue->setValue(currentRect.y());
                    ui->widthEtalonValue->setValue(currentRect.width());
                    ui->heightEtalonValue->setValue(currentRect.height());
                }
            }
        }
        painter.drawPixmap(0, 0, currentPix);
    }
    painter.end();
}

void Widget::on_setEtalonHandle_clicked()
{
    isSetEtalonHandle = true;
    ui->setEtalonHandle->setDown(true);
}
void Widget::on_setEtalonFromCoordinates_clicked()
{
    isSetEtalonHandle = true;
    ui->setEtalonFromCoordinates->setDown(true);
}

void Widget::on_saveEtalon_clicked()
{
    saveEtalon = true;
    isSetEtalonHandle = false;
    isSetEtalonFromCoordinates = false;

    currentRect.setX(ui->xEtalonValue->value());
    currentRect.setY(ui->yEtalonValue->value());
    currentRect.setWidth(ui->widthEtalonValue->value());
    currentRect.setHeight(ui->heightEtalonValue->value());
    createNewQPixmapEtalon();
    createNewMatEtalon();

    QPainter tempPainter(&currentPix);
    tempPainter.setPen(QColor(255, 0, 0, 255));
    tempPainter.drawRect(roiRect);
    tempPainter.end();

    updateRoi();
    repaint();
}

void Widget::on_fileDialogButton_clicked()
{
    // Запрашиваем у пользователя видеопоследовательность
    vector<String> imageFilenames = getImageFilenames();
    // Выгружаем в вектор для хранения картинок
    loadImagesFromPath(imageFilenames);

    // доработать
    currentMat = videoSequence[0];
    currentPix = Mat2QPixmap(currentMat);
    update();
    waitKey(10);

}

void Widget::loadImagesFromPath(vector<String> imgFilenames){
    Mat image;
    videoSequence.clear();
    for (size_t i=1; i < imgFilenames.size(); i++){
        image = imread(imgFilenames[i], IMREAD_GRAYSCALE);
        cv::resize(image, image, Size(imageHeight, imageWidth));
        videoSequence.push_back(image);
    }
    cout << "Загружено изображений " << videoSequence.size() << endl;
}

vector<String> Widget::getImageFilenames(){

    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите папку"),
                                                    "/home/liza/Pictures",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    // Формирование последовательности картинок из пути папки
    vector<cv::String> filenames;
    String pngImagePath = (dir + "/*.jpg").toStdString();
    glob(pngImagePath, filenames, false);
    return filenames;
}

void Widget::on_startTracking_clicked()
{
    Criterion_function_evaluator* cryteryFunction = new Criterion_function_evaluator();
    Etalon_updater* etalonUpdayer = new Etalon_updater();

    // тут соединения всех кодов
    for(Mat image: videoSequence){
        currentPix = Mat2QPixmap(image);
        currentMat = image;

        // тут вставить код Ильи и Миши
        Mat debugMat = cryteryFunction->calculation_criterion(roiMat, etalonMat);
        QRect outputData = etalonUpdayer->search(etalonMat, debugMat);
        currentRect.setX(outputData.x() + roiRect.x());
        currentRect.setY(outputData.y() + roiRect.y());
        currentRect.setWidth(outputData.width());
        currentRect.setHeight(outputData.height());

        imwrite("/home/liza/Desktop/etalon.png", etalonMat);
        imwrite("/home/liza/Desktop/debug.png", debugMat);
        imwrite("/home/liza/Desktop/roi.png", roiMat);

        QPainter tempPainter(&currentPix);
        //                tempPainter.begin(this);
        //        tempPainter.setPen(QColor(0, 247, 28, 255));
        //        tempPainter.drawRect(currentRect);
        tempPainter.setPen(QColor(255, 0, 0, 255));
        tempPainter.drawRect(roiRect);
        tempPainter.end();

        // Обновление эталона
        Mat new_etalon = etalonMat.clone();
        currentMat(cv::Rect(currentRect.x(),currentRect.y(),
                            currentRect.width(),currentRect.height())).copyTo(new_etalon);

        float betta = 0.3; //коэфиициент сглаживания
        float updateN = 0;
        for (int i = 0; i <= etalonMat.rows; i++){
            for (int j = 0; j <= etalonMat.cols; j++){
//                cout << (float)etalonMat.at<uchar>(i,j) << endl;
                updateN = (betta * (float)etalonMat.at<uchar>(i,j))
                        +((1-betta) * (float)new_etalon.at<uchar>(i,j));

                if (updateN > 255)
                    updateN = 255;
                if (updateN < 0)
                    updateN = 0;
                etalonMat.at<float>(i,j) = updateN;
            }
        }
        createNewMatEtalon();
        createNewQPixmapEtalon();
        updateRoi();

        if(isStop){
            isStop = false;
            break;
        }
        update();
        waitKey(10);
    }
}

void Widget::updateRoi(){

    int width = currentRect.width();
    int height = currentRect.height();
    int xCoor =currentRect.x();
    int yCoor =currentRect.y();

    int nx = width/2; //число на которое увеличиваем ROI относительно эталона
    int ny = height/2;

    int width_roi = width+2*nx;
    int height_roi = height+2*ny;

    int nx_down = xCoor-nx + width_roi;
    int ny_down = yCoor-ny + height_roi; //координаты нижнего угла

    if (nx_down  > imageHeight){
        nx_down  =imageHeight - xCoor + nx;
        width_roi = nx_down;
    }
    if (ny_down > imageWidth){
        ny_down = imageWidth - yCoor + ny;
        height_roi = ny_down;
    }

    roiRect = QRect(xCoor-nx,yCoor-ny,width_roi,height_roi);
    currentMat(cv::Rect(roiRect.x(),roiRect.y(),
                        roiRect.width(),roiRect.height())).copyTo(roiMat);
}

void Widget::on_stopTracking_clicked()
{
    isStop = true;
}

void Widget::on_saveNoiseSettingsButton_clicked()
{
    blurValue = ui->blurValue->value();
    noiseValue = ui->noiseValue->value();
    noiseType = ui->noiseType->currentText();

    // тут Настино размытие
}

void Widget::on_saveCryterySettingsButton_clicked()
{
    cryteryFunctionType = ui->crytheryType->currentText();
    cout << cryteryFunctionType.toStdString() << endl;
}


