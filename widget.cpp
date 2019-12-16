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

QPixmap Widget::Mat2QPixmap(cv::Mat const& src)
{
    cv::Mat temp = src.clone();
    return QPixmap::fromImage(QImage((uchar*) temp.data, temp.cols,
                                     temp.rows, temp.step, QImage::Format_Grayscale8));
}

void Widget::createNewMatEtalon()
{
    originalMat(cv::Rect(currentRect.x(),currentRect.y(),
                         currentRect.width(),currentRect.height())).copyTo(etalonMat);
}

void Widget::createNewQPixmapEtalon()
{
    etalonPix = originalPix.copy(currentRect);
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
        currentPix = originalPix.copy();

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
            createNewQPixmapEtalon();
            createNewMatEtalon();
            if (not isSetEtalonFromCoordinates){
                ui->xEtalonValue->setValue(currentRect.x());
                ui->yEtalonValue->setValue(currentRect.y());
                ui->widthEtalonValue->setValue(currentRect.width());
                ui->heightEtalonValue->setValue(currentRect.height());
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
    isSetEtalonFromCoordinates = true;
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

    currentPix = originalPix.copy();
    painter.setPen(QColor(255, 247, 28, 255));
    painter.drawRect(currentRect);
    update();
    updateRoi();
}

void Widget::on_fileDialogButton_clicked()
{
    // Запрашиваем у пользователя видеопоследовательность
    vector<String> imageFilenames = getImageFilenames();
    // Выгружаем в вектор для хранения картинок
    loadImagesFromPath(imageFilenames);

    // доработать
    originalMat = videoSequence[0];
    originalPix = Mat2QPixmap(originalMat) ;
    currentPix = originalPix.copy();
    update();
}

void Widget::loadImagesFromPath(vector<String> imgFilenames){
    Mat image;
    videoSequence.clear();
    for (size_t i=0; i < imgFilenames.size(); i++){
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
    Criterion_function_evaluator cryteryFunction;
    Etalon_updater etalonUpdayer;
    // тут соединения всех кодов
    for(Mat image: videoSequence){
        currentPix = Mat2QPixmap(image);

        // тут вставить код Ильи и Миши
        Mat debugMat = cryteryFunction.calculation_criterion(image, etalonMat);
        vector<QRect> outputData = etalonUpdayer.search(etalonMat, debugMat);
        currentRect = outputData[0];
        roiRect = outputData[1];

        // Обновление эталона
        createNewMatEtalon();
        createNewQPixmapEtalon();

        //        currentPix = originalPix.copy();
        QPainter tempPainter(&currentPix);
        tempPainter.setPen(QColor(0, 247, 28, 255));
        tempPainter.drawRect(currentRect);

        update();
        waitKey(100);

        if(isStop){
            isStop = false;
            break;
        }
    }
}
void Widget::updateRoi(){

    int width = currentRect.width();
    int height = currentRect.height();
    int xCoor =currentRect.x();
    int yCoor =currentRect.y();


    int nx = width/4; //число на которое увеличиваем ROI относительно эталона
    int ny = height/4;

    int width_roi = width+2*nx;
    int height_roi = height+2*ny;

    int nx_down = xCoor-nx + width_roi;
    int ny_down = yCoor-ny + height_roi; //координаты нижнего угла

    cout <<"Ширина рои " <<width_roi <<" Длина рои " << height_roi<<endl;
    cout <<"Координаты нижнего угла Х " <<nx_down <<" У " << ny_down<<endl;

    if (nx_down  > imageHeight){
        nx_down  =imageHeight - xCoor + nx;
        width_roi = nx_down;
    }
    if (ny_down > imageWidth){
        ny_down = imageWidth - yCoor + ny;
        height_roi = ny_down;
    }
    //cout << nx_down <<"    " << ny_down<<endl;
//    cout << "Измененная длина и ширина рои "<< width_roi <<"    " << height_roi<<endl;

    roiRect = QRect(xCoor-nx,yCoor-ny,width_roi,height_roi);

    QPainter tempPainter(&currentPix);
    tempPainter.setPen(QColor(255, 0, 0, 255));
    tempPainter.drawRect(roiRect);
    update();
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


