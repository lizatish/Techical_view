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

    isRanMainThread = false;
}

Widget::~Widget()
{
    delete ui;
}

QPixmap Widget::Mat2QPixmap(const cv::Mat& src)
{
    QImage image((unsigned char*) src.data, src.cols, src.rows,
                 static_cast<int>(src.step),QImage::Format_Grayscale8);
    QPixmap pix = QPixmap::fromImage(image);
    return pix;
}

void Widget::createNewMatEtalon()
{
    etalonMat = currentMat(cv::Rect(currentRect.x(),currentRect.y(),
                                    currentRect.width(),currentRect.height())).clone();
}

void Widget::createNewQPixmapEtalon()
{
    etalonPix = Mat2QPixmap(etalonMat);/*currentPix.copy(currentRect);*/
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
            painter.drawRect(currentRect);
        }
        drawStarted = true;
    }
    else if (drawStarted){
        QPainter tempPainter(&currentPix);
        if(currentRect.width() > 3 and currentRect.height() > 3){
            tempPainter.setPen(QColor(255, 247, 28, 255));
            tempPainter.drawRect(currentRect);
            if (isExistRoi){
                tempPainter.setPen(QColor(255, 0, 0, 255));
                tempPainter.drawRect(roiRect);
            }
            createNewQPixmapEtalon();
            createNewMatEtalon();
        }
        painter.drawPixmap(0, 0, currentPix);
    }

    if (not isSetEtalonFromCoordinates){
        ui->xEtalonValue->setValue(currentRect.x());
        ui->yEtalonValue->setValue(currentRect.y());
        ui->widthEtalonValue->setValue(currentRect.width());
        ui->heightEtalonValue->setValue(currentRect.height());
    }
    painter.end();
}

void Widget::on_setEtalonHandle_clicked()
{
    isSetEtalonHandle = true;
    isExistRoi = false;
    ui->setEtalonHandle->setDown(true);
}
void Widget::on_setEtalonFromCoordinates_clicked()
{
    isSetEtalonFromCoordinates = true;
    isExistRoi = false;
    ui->setEtalonFromCoordinates->setDown(true);
}

void Widget::on_saveEtalon_clicked()
{
    saveEtalon = true;
    isSetEtalonHandle = false;
    isSetEtalonFromCoordinates = false;
    isExistRoi = true;

    currentRect.setX(ui->xEtalonValue->value());
    currentRect.setY(ui->yEtalonValue->value());
    currentRect.setWidth(ui->widthEtalonValue->value());
    currentRect.setHeight(ui->heightEtalonValue->value());
    updateRoi();
    update();
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
    isRanMainThread = true;
    Criterion_function_evaluator* cryteryFunction = new Criterion_function_evaluator();
    image_processing* I = new image_processing();

    int noiseType = 0, blurType = 0;
    float noiseValue = 0, blurValue = 0;

    //Выбираемметод,которымбудемсопоставлять
    //0:SQDIFF 1:SQDIFFNORMED 2:TMCCORR 3:TMCCORRNORMED 4:TMCOEFF 5:TMCOEFFNORMED
    const int CRYTERION_FUNCTION_METHOD = 4;

    for(Mat image: videoSequence){
        currentMat = I->addNoiseAndBlur(image.clone(), noiseType, blurType, noiseValue, blurValue);
        currentPix = Mat2QPixmap(currentMat);
        updateRoi();

        Mat debugMat = cryteryFunction->calculateCriterionFunction(roiMat, etalonMat, CRYTERION_FUNCTION_METHOD);

        currentRect = cryteryFunction->getEtalonCoordinates(debugMat, CRYTERION_FUNCTION_METHOD, roiRect, etalonMat);
        update();
        waitKey(10);
    }
}

void Widget::updateRoi(){

    int deltaX = currentRect.width() / 2; //число на которое увеличиваем ROI относительно эталона
    int deltaY = currentRect.height() / 2;

    int x1Roi = currentRect.x() - deltaX;
    int y1Roi = currentRect.y() - deltaY;
    int x2Roi = currentRect.x() + currentRect.width() + deltaX;
    int y2Roi = currentRect.y() + currentRect.height() + deltaY;

    if (x1Roi < 0){
        x1Roi = 0;
    }
    if (y1Roi < 0){
        y1Roi = 0;
    }
    if (x2Roi > imageHeight){
        x2Roi = imageHeight;
    }
    if (y2Roi > imageWidth){
        y2Roi = imageWidth;
    }

    roiRect = QRect(x1Roi, y1Roi, x2Roi - x1Roi, y2Roi - y1Roi);
    roiMat =  currentMat(cv::Rect(roiRect.x(),roiRect.y(),
                                  roiRect.width(),roiRect.height())).clone();
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
}

void Widget::on_saveCryterySettingsButton_clicked()
{
    cryteryFunctionType = ui->crytheryType->currentText();
}


