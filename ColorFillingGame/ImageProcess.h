#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>


class ImageProcess
{
public:
	ImageProcess();
	~ImageProcess();
	
	cv::Mat getImageByFilePath(std::string filePath);
private:


};

