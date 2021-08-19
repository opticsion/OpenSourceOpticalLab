#include <string.h>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>
#include "pixel.hpp"
#include <iostream>
#include <stdio.h>
#include <opencv2/imgproc/imgproc.hpp>



typedef struct{
	float beamCenterX;
	float beamCenterY;
	float momentX;
	float momentY;
	float momentXY;
	float diamX;
	float diamY;
	float meanDiam;
}laserSpec;


class FocalMeasure{
	private:
		laserSpec beam[2];
		double pixelSize;
		Pixel* min = new Pixel();
		Pixel* max = new Pixel();
		int thresholdX[4]={0,0,0,0};
		int thresholdY[2]={0,0};
		int center1;
		int center2;
		int centerY;
		bool error;
		float distance;
	public:
		FocalMeasure(double pxSize);
		void moment(cv::Mat* frameG, int beamNbr);
		float power(cv::Mat* frameG);
		std::string strCentX();
		std::string strCentY();
		cv::Point getCent1();
		cv::Point getCent2();
		int getPow();
		std::string getStrPow();
		int getRadius1();
		int getRadius2();
		cv::Mat profileX(cv::Mat* frame,Pixel* pixel,float* mean,float* standard_deviation,float* dataProfile,
					float* widthProfileX,double* limitWidth,int* filterPrecision, double pixSize);
		cv::Mat profileY(cv::Mat* frame,Pixel* pixel,float* mean,float* standard_deviation,float* dataProfile,
					float* widthProfileY,double* limitWidth,int* filterPrecision);
		void calcDistanceX(cv::Mat* frameG);
		void calcDistanceY(cv::Mat* frameG);
		Pixel* maximum(cv::Mat* frameG);
		void maxi(cv::Mat* frameG);
		Pixel* getMax();
		int* getThresholdX();
		int* getThresholdY();
		void findDistance(cv::Mat* frameG);
		bool getStatus();
		cv::Point getMomentCent(int nb);
		std::string getStrDistance();
};
