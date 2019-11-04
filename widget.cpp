#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // запилить норм функцию и продумать над последовательностью
    //    originalMat = imread("/home/liza/!QTProjects/technical_view/fox2.jpg", IMREAD_GRAYSCALE);
    //    cv::resize(originalMat, originalMat, Size(640, 480));
    //    originalPix = Mat2QPixmap(originalMat) ;
    //    currentPix = originalPix.copy();

    mousePressed = true;
    drawStarted = false;
    saveEtalon = false;
    changeEtalon = false;
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
    // Открыть диалоговое окно
    ui->fileDialogButton->setToolTip(tr("Open image for create video dequence"));
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load picture"), "",
                                                    tr("All Files (*)"));
    // Парсинг пути и вынимание папки
    QString folder_name = QFileInfo(fileName).baseName();
    folder_name.append(".files");
    QDir tmp_dir(QFileInfo(fileName).dir());

    // Формирование последовательности картинок из пути папки
    vector<cv::String> filenames;
    String pngImagePath = (tmp_dir.absolutePath() + "/*.png").toStdString();
    glob(pngImagePath, filenames, false);
    return filenames;
}
