#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ColorFillingGame.h"
#include <opencv2/opencv.hpp>
class ColorFillingGame : public QMainWindow
{
    Q_OBJECT

public:
    ColorFillingGame(QWidget *parent = nullptr);
    ~ColorFillingGame();

private:
    void showImage();

    void setLabelShowImage(unsigned char* data,int width,int height,int channels);
    void setLabelResolution(int width, int height);

    void loadWidgetColor(std::string path);
private:
    cv::Mat _srcImage;
    cv::Mat _curImage;
private:
    Ui::ColorFillingGameClass ui;

public slots:
    void on_btn_threshold_clicked();
    void on_btn_reset_clicked();
    void on_btn_adaptive_threshold_clicked();
    void on_btn_remove_small_centroids_clicked();
};
