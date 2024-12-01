#include "ColorFillingGame.h"
#include <opencv2/opencv.hpp>
#include <qpixmap.h>
#include <qimage.h>
#include <fstream>
#include <sstream>
#include <qgridlayout.h>

ColorFillingGame::ColorFillingGame(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    std::string filePath = "./images/wuxia/1.png";
    _srcImage = cv::imread(filePath);
    showImage();

    loadWidgetColor("./colors.txt");
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

    cv::resize(_curImage, _curImage, cv::Size(labelWidth, labelHeight));
    cv::Mat rgbImage;
    cv::cvtColor(_curImage, rgbImage, cv::COLOR_BGR2RGB);
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

void ColorFillingGame::loadWidgetColor(std::string path)
{
    std::ifstream infile(path);
    if (!infile.is_open()) {
        return;
    }

    std::string line;
    char color[7];
    std::vector<std::string> colors;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        color[0] = '#';
        if (iss >> color[1] >> color[2] >> color[3] >> color[4] >> color[5] >> color[6]) {
            colors.push_back(std::string(color, 7));
        }
    }

    infile.close();

    int cols = 7;
    int rows = 4;
    if (colors.size() / cols > 4) {
        rows = 4;
    }
    else {
        rows = colors.size() / cols;
    }

    QGridLayout* gridLayout = new QGridLayout(ui.widget_colors);

    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            QLabel* label = new QLabel(" ");
            label->setFrameShadow(QFrame::Sunken);
            label->setFrameShape(QFrame::Panel);
            std::string stylesheet = "background-color: " + colors[row * cols + col];
            label->setStyleSheet(QString::fromStdString(stylesheet));

            gridLayout->addWidget(label, row, col);
        }
    }
}

void ColorFillingGame::on_btn_reset_clicked()
{
    _curImage = _srcImage.clone();
    cv::resize(_curImage, _curImage, cv::Size(ui.label_show_img->width(), ui.label_show_img->height()));
    setLabelShowImage(_curImage.data, _curImage.cols, _curImage.rows, _curImage.channels());
}

void ColorFillingGame::on_btn_adaptive_threshold_clicked()
{
    cv::Mat adaptiveImage;
    cv::cvtColor(_curImage, _curImage, cv::COLOR_RGB2GRAY);
    cv::adaptiveThreshold(_curImage, adaptiveImage, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 11, 2);
    _curImage = adaptiveImage.clone();
    setLabelShowImage(_curImage.data, _curImage.cols, _curImage.rows, _curImage.channels()); 
}

void ColorFillingGame::on_btn_remove_small_centroids_clicked()
{
    cv::bitwise_not(_curImage, _curImage);

    int nLabels = 0;
    cv::Mat labels, stats, centroids;
    int connectivity = 8;
    nLabels = cv::connectedComponentsWithStats(_curImage, labels, stats, centroids, connectivity);
    int minArea = 100;

    cv::Mat output = cv::Mat::zeros(_curImage.size(), CV_8UC1);

    for (int y = 0; y < labels.rows; ++y) {
        for (int x = 0; x < labels.cols; ++x) {
            int label = labels.at<int>(y, x);
            if (label == 0 || (label > 0 && stats.at<int>(label, cv::CC_STAT_AREA) < minArea)) {
                output.at<uchar>(y, x) = 0;
            }
            else {
                output.at<uchar>(y, x) = 255;
            }
        }
    }

    cv::bitwise_not(output, output);
    _curImage = output.clone();
    setLabelShowImage(_curImage.data, _curImage.cols, _curImage.rows, _curImage.channels());
}

void ColorFillingGame::on_btn_threshold_clicked() {
    
    int value = std::atoi(ui.lineEdit_threshold_value->text().toStdString().c_str());
    cv::Mat thrImage(cv::Size(_curImage.cols, _curImage.rows), CV_8UC1);
    cv::cvtColor(_curImage, thrImage, cv::COLOR_BGR2GRAY);
    cv::threshold(thrImage, thrImage, value, 255, cv::THRESH_BINARY);

    setLabelShowImage(thrImage.data, thrImage.cols, thrImage.rows, thrImage.channels());
    setLabelResolution(thrImage.cols, thrImage.rows);
}