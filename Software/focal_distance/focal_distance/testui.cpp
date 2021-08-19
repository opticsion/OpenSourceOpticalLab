/*
This is a demo application to showcase the image button component.
Copyright (c) 2016 Fernando Bevilacqua <dovyski@gmail.com>
Licensed under the MIT license.
*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define CVUI_IMPLEMENTATION
#include "cvui.h"

#define WINDOW_NAME	"Image button"

int main(int argc, const char *argv[])
{
	cv::Mat frame = cv::Mat(1400, 1800, CV_8UC3);
	cv::Mat out = cv::imread("image.png", cv::IMREAD_COLOR);
	cv::Mat down = cv::imread("./lena-face-red.jpg", cv::IMREAD_COLOR);
	cv::Mat over = cv::imread("./lena-face-gray.jpg", cv::IMREAD_COLOR);

	// Init cvui and tell it to create a OpenCV window, i.e. cv::namedWindow(WINDOW_NAME).
	cvui::init(WINDOW_NAME);
	
	while (true) {
		// Fill the frame with a nice color
		frame = cv::Scalar(49, 52, 49);
		cvui::image(frame,10,10,out);
		
		
		
		
		// This function must be called *AFTER* all UI components. It does
		// all the behind the scenes magic to handle mouse clicks, etc.
		cvui::update();
		
		// Show everything on the screen
		cv::imshow(WINDOW_NAME, frame);
		cv::imshow(WINDOW_NAME, frame);
		// Check if ESC key was pressed
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
	
	return 0;
}
