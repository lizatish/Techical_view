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
    changeEtalon = false;
    isStop = false;
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
    if(event->type() == QEvent::MouseMove and changeEtalon){
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
            painter.drawRect(currentRect);
            createNewQPixmapEtalon();
            createNewMatEtalon();
        }
        drawStarted = true;
    }
    else if (drawStarted){
        QPainter tempPainter(&currentPix);
        if(currentRect.width() > 3 and currentRect.height() > 3){
            tempPainter.drawRect(currentRect);
            // Меняем эталоны
            createNewQPixmapEtalon();
            createNewMatEtalon();
        }
        painter.drawPixmap(0, 0, currentPix);
    }
    painter.end();
}

void Widget::on_changeEtalon_clicked()
{
    changeEtalon = true;
    ui->changeEtalon->setDown(true);
}

void Widget::on_saveEtalon_clicked()
{
    saveEtalon = true;
    changeEtalon = false;
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

// добавить считывание ток картинок с тока папки
vector<String> Widget::getImageFilenames(){

    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите папку"),
                                                    "/home/liza/Pictures",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);

    // Формирование последовательности картинок из пути папки
    vector<cv::String> filenames;
    String pngImagePath = (dir + "/*.png").toStdString();
    glob(pngImagePath, filenames, false);
    return filenames;
}

void Widget::on_saveButton_clicked()
{
    blurValue = ui->blurValue->value();
    noiseValue = ui->noiseValue->value();
    noiseType = ui->noiseType->currentText();

    // тут Настино размытие
}

void Widget::on_startTracking_clicked()
{
    // тут соединения всех кодов
    for(Mat image: videoSequence){
        currentPix = Mat2QPixmap(image);

        // тут вставить код Ильи и Миши

        // тут замена эталона и координат boundRect

        update();
        waitKey(100);

        if(isStop){
            isStop = false;
            break;
        }
    }
}

void Widget::on_stopTracking_clicked()
{
    isStop = true;
}
