#include "ColorFillingGame.h"
#include <opencv2/opencv.hpp>
#include <qpixmap.h>
#include <qimage.h>
ColorFillingGame::ColorFillingGame(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    std::string filePath = "./images/wuxia/1.png";
    _srcImage = cv::imread(filePath);
    showImage();
}

ColorFillingGame::~ColorFillingGame()
{


}

void ColorFillingGame::showImage()
{
    _curImage = _srcImage.clone();

    int labelWidth = ui.label_show_img->width();
    int labelHeight = ui.label_show_img->height();

    //ui.label_show_img->setPixmap();
    cv::cvtColor(_curImage, _curImage, cv::COLOR_BGR2RGB);
    cv::resize(_curImage, _curImage, cv::Size(labelWidth, labelHeight));
    QImage image = QImage(_curImage.data, labelWidth, labelHeight, labelWidth * 3, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(image);
    ui.label_show_img->setPixmap(pixmap);

    std::string strResoultion = std::to_string(labelWidth) + " x " + std::to_string(labelHeight);
    QString resoultion = QString::fromStdString(strResoultion);
    ui.label_resolution->setText(resoultion);
}

void ColorFillingGame::setLabelShowImage(unsigned char* data, int width, int height, int channels)
{
    if (channels == 1) {
        QImage image = QImage(data, width, height, width * channels, QImage::Format_Grayscale8);
        QPixmap pixmap = QPixmap::fromImage(image);
        ui.label_show_img->setPixmap(pixmap);
    }
    else if (channels == 3) {
        QImage image = QImage(data, width, height, width * channels, QImage::Format_RGB888);
        QPixmap pixmap = QPixmap::fromImage(image);
        ui.label_show_img->setPixmap(pixmap);
    }
    else {
        std::cout << "unsupport mode.\n";
    }
}

void ColorFillingGame::setLabelResolution(int width, int height)
{
    std::string strResoultion = std::to_string(width) + " x " + std::to_string(height);
    QString resoultion = QString::fromStdString(strResoultion);
    ui.label_resolution->setText(resoultion);
}

void ColorFillingGame::on_btn_threshold_clicked() {
    
    int value = std::atoi(ui.lineEdit_threshold_value->text().toStdString().c_str());
    cv::Mat thrImage(cv::Size(_curImage.cols, _curImage.rows), CV_8UC1);
    cv::cvtColor(_curImage, thrImage, cv::COLOR_BGR2GRAY);
    cv::threshold(thrImage, thrImage, value, 255, cv::THRESH_BINARY);

    setLabelShowImage(thrImage.data, thrImage.cols, thrImage.rows, thrImage.channels());
    setLabelResolution(thrImage.cols, thrImage.rows);
}