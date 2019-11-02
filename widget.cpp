#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // запилить норм функцию и продумать над последовательностью
    originalMat = imread("/home/liza/!QTProjects/technical_view/fox2.jpg", IMREAD_GRAYSCALE);
    cv::resize(originalMat, originalMat, Size(640, 480));
    originalPix = Mat2QPixmap(originalMat) ;
    currentPix = originalPix.copy();

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
    ui->etalon->setMaximumSize(QSize(ui->etalon->x(), ui->etalon->y()));
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
        currentRect.setBottomRight(event->pos());
    }
    update();
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
    ui->fileDialogButton->setToolTip(tr("Load contacts from a file"));
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load picture"), "",
                                                    tr("All Files (*)"));
    qDebug(fileName.toLatin1());

    QFileInfo fi(fileName);
    QString folder_name = fi.baseName();
    folder_name.append(".files");  // "jpg.files"

    QDir tmp_dir(fi.dir());// "c:\\myFolder\\"
    tmp_dir.mkdir(folder_name); // "c:\\myFolder\\jpg.files\\"

    QStringList images = tmp_dir.entryList(QStringList() << "*.png" << "*.png",QDir::Files);

    foreach(QString filename, images) {
        vector<cv::String> fn;
        QString s = tmp_dir.absolutePath() + "/*.png";
        //        String s = tmp_dir. + "4";
        //        QString absolute_file_path = tmp_dir.absoluteFilePath(relative_file_path);

        //                glob("/home/images/*.png", fn, false);
        //        s1 = s.toLatin1();
        String s1 = s.toStdString();
        glob(s1, fn, false);

        vector<Mat> imags;
        for (size_t i=0; i < fn.size(); i++){
            cout << fn[i].c_str() << endl;
            String fname = fn[i].c_str();
            Mat imgg = imread(fname, IMREAD_GRAYSCALE);
            imags.push_back(imgg);
            imshow("12", imgg );

            waitKey(100);
        }
    }
}
