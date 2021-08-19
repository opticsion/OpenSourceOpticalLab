/* This file is the light main.cpp. please find more fonctions and detail in mainextend.cpp */
/* Copyright Martial Geiser, Alex Miollany, Christopher Tremblay & Brice Dubuis */
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <unistd.h>
#include "time.hpp"
#include "display.hpp"
#include "camera.hpp"
#include "gradientframe.hpp"
#include "focalMeasure.hpp"
#include "cvplot.hpp"
#include <cmath>
#include <raspicam/raspicam_cv.h>
#define CVUI_IMPLEMENTATION
#include "cvui.h"
#define WINDOW_NAME "Laser profiler"
#include <fstream>
#include <dirent.h>

#include <iomanip>
#include <string>

using namespace cv;
using namespace std;
using namespace CvPlot;

/*===============================================================================
	define variables
	This global variables are for the use of the Camera Pi V2
================================================================================*/
#define APP_VERSION 1.0
#define RESOLUTION_H 960
#define RESOLUTION_W 1280
#define CAM_RES_H 3040
#define CAM_RES_W 4056

enum State{ST_STARTPAGE, ST_TWOLIGHTPOINTS, ST_1STLIHGHTPOINT, ST_2NDLIGHTPOINT, ST_DISPLAY_RESULT, ST_ABOUT};
enum Language{ENGLISH, FRENCH, GERMAN};
enum DisplayType{ABSOLUTE, RELATIVE, NORMAL};

//compilation : g++ $(pkg-config --cflags --libs opencv) -lraspicam_cv -lraspicam main.cpp cvplot.cpp pixel.cpp time.cpp display.cpp camera.cpp gradientframe.cpp focalMeasure.cpp -o FocalDistance -Wno-psabi
// pkg-config is used to link the program to library

Mat principalFrame;		//Full window displayed on the screen
Display* myDisplay= new Display(&principalFrame);

int main(int argc, char** argv){
  Mat frame= Mat(960,1280,CV_8UC3);	// frame=framec  --> display the maximum without interfering the following measures
  Mat framec;
  Mat frameT;    					// frameT --> display the maximum without interfering the following measures

  Mat frameAc = Mat(320,426,CV_8UC3);	//Camera feedback display
  Mat frameScale = Mat(15,426,CV_8U);	//image that show the color scale
  Mat frameG;				//image captured converted in grayscale
  Mat frameP1,frameP11,frameP2;		//image that show the profile X
  frameP11=Mat(160,426,CV_8U);	//frame x (rows,cols,type)

  float* meanY;		//Average by line
  float* standard_deviationY;
  float* meanX;		//Average by row
  float* standard_deviationX;
  float* widthProfileX=(float*)malloc(sizeof(float));	//treatment result : diameter by X
  float* widthProfileY=(float*)malloc(sizeof(float));	// diameter by Y
  float* diam=(float *)malloc(sizeof(float) *8 );		//  x_bar,y_bar,moment_x,moment_y,moment_xy,diam_x,diam_y,mean_diam
  int* rgb=(int *)malloc(sizeof(int) *3 );
  int filterPrecision=32;
  meanY=(float*)malloc(sizeof(float));
  standard_deviationY=(float*)malloc(sizeof(float));
  meanX=(float*)malloc(sizeof(float));
  standard_deviationX=(float*)malloc(sizeof(float));
  float* dataProfileX=(float*)malloc(RESOLUTION_W*sizeof(float));
  float* dataProfileY=(float*)malloc(RESOLUTION_H*sizeof(float));
  double limitWidth=13.5;
  *widthProfileX=0;
  *widthProfileY=0;

  State state=ST_STARTPAGE;	//record the state of the state machine
  DisplayType display=RELATIVE;		//select the display mode of the image 
  Language language=ENGLISH;
  Point textOrg;
  cvNamedWindow(WINDOW_NAME, CV_WINDOW_NORMAL);
  cvSetWindowProperty(WINDOW_NAME, CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
  cvui::init(WINDOW_NAME);
  Pixel* max;
  ofstream myfile;	//file that describe the programm in About mode
  raspicam::RaspiCam_Cv cap;	//Initialization of the camera
  cap.set(CV_CAP_PROP_FORMAT, CV_8UC3);	//set format
  if (!cap.open()) {cerr<<"Error opening the camera"<<endl;return -1;}	//open camera
  
  cout << cap.get(CV_CAP_PROP_FRAME_WIDTH)<<"x"<<cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;	//dimension of image from camera
  cout << "Start grabbing" << endl
      << "Press any key to terminate" << endl;
  
  Time myTime;
  string fileName = "";
  size_t posX, posY;
  bool calc_moment=false;
  int *thresholdX;
  int *thresholdY;
  Camera* myCamera= new Camera(&cap);
  myCamera->setParameters();
  GradientFrame* theImage= new GradientFrame();
  Display* myDisplay= new Display(&principalFrame);
  FocalMeasure* myLaser = new FocalMeasure(myCamera->getPixelSize());
  Mat image = imread("ColorScale.PNG",CV_LOAD_IMAGE_COLOR);
  for (;;){
  	principalFrame = cv::Mat(480,800,CV_8UC3);  //Creation of the main frame
  	principalFrame.setTo(cv::Scalar(49,52,49));	//Set background color
	switch(state){      //No button pressed, enter in play mode
	case ST_STARTPAGE:
		cap.grab();				//Wait for a new frame from camera and store it into 'frame'
		cap.retrieve(frameT);	//Capturing the image
		resize(frameT,frame,frameT.size(),0,0,INTER_NEAREST);
		rotate(frame,frame,3);
		cvtColor(frame,frameG, CV_BGR2GRAY);		//convert from a normal image to a grayscale image
		max=myLaser->maximum(&frameG);
		cap.retrieve(framec);          //Capturing the image
		if(display!=NORMAL){
			int c;
			for(int i=0;i<framec.rows;i++){
				for(int j=0;j<framec.cols*3;j+=3){
					c=frameG.at<uchar>(i,j/3);
					if(display==RELATIVE) theImage->gradient_relative(c,rgb,max->getIntensity());
					else if(display==ABSOLUTE) theImage->gradient_absolute(c,rgb);
					framec.at<uchar>(i,j)=rgb[2];
					framec.at<uchar>(i,j+1)=rgb[1];
					framec.at<uchar>(i,j+2)=rgb[0];
				}
			}
		}

		frameP2=myLaser->profileY(&frameG,max,meanY,standard_deviationY,dataProfileY,widthProfileY,&limitWidth,&filterPrecision);
		frameP1=myLaser->profileX(&frameG,max,meanX,standard_deviationX,dataProfileX,widthProfileX,&limitWidth,&filterPrecision,myCamera->getPixelSize());
		resize(frameP1,frameP11,frameP11.size(),0,0,INTER_NEAREST);	//frameP1 to size of frameP11
		cvui::image(principalFrame,0,320,frameP11);//PROFILE X
		myLaser->maxi(&frameG);		//update the max
		myLaser->calcDistanceX(&frameG);
		myLaser->calcDistanceY(&frameG);
		thresholdX=myLaser->getThresholdX();
		thresholdY=myLaser->getThresholdY();

		resize(framec,frameAc,frameAc.size(),0,0,INTER_NEAREST);
		cvui::image(principalFrame,0,0,frameAc);     //Adding the frame to the interface

		if(display!=NORMAL){
			resize(image,frameScale,frameScale.size(),0,0,INTER_NEAREST);	//sets the colour scale to fit size
			cvui::image(principalFrame,0,0,frameScale);
		}

		if(*meanX>=0 && *meanX<=1){
			myDisplay->addText(436,430,"X Profile:");
			String str1 = to_string((max->getX()-RESOLUTION_W/2)*2*myCamera->getPixelSize());
			String valX = str1.substr(0,str1.find(".")+2);	//set the good format to number
			String xSpec = "Width: ~"+ to_string(int(*widthProfileX)) + " microns @ X="+ valX+"um";
			myDisplay->addText(436,450,xSpec);
		}
		filterPrecision=myDisplay->add_cmp(530,200,120,20,filterPrecision);
		limitWidth=myDisplay->add_switch(580,270,50,20,limitWidth);	//return 13.5 or 50
		
		switch(display){
		case RELATIVE:
			myDisplay->addText(500,330, "Display mode: relative");
			if(cvui::button(principalFrame,500,350,"Change Display Mode")) display=ABSOLUTE;
		break;
		case ABSOLUTE:
			myDisplay->addText(500,330,"Display mode: absolute");
			if(cvui::button(principalFrame,500,350,"Change Display Mode")) display=NORMAL;
		break;
		case NORMAL:
			myDisplay->addText(500,330,"Display mode: normal");
			if(cvui::button(principalFrame,500,350,"Change Display Mode")) display=RELATIVE;
		break;
		default:
			myDisplay->addText(500,330, "Display mode: relative");
			if(cvui::button(principalFrame,500,350,"Change Display Mode")) display=RELATIVE;
		break;
		}	//end switch
		
		if(cvui::button(principalFrame,630,15,"About")){
			state=ST_ABOUT;          //It switches back to play mode every time the about mode is exited
		}
		if(cvui::button(principalFrame,440,15,"2 Lightpoints")){
			fileName=myTime.getTime();
			std::string nomCsv=fileName+".csv";          //CSV file of profiles
			std::string nomImage=fileName+"_image.png";      //Image of the laser
			myLaser->calcDistanceX(&frameG);
			myfile.open ("./measurments/"+nomCsv);
			if(&myfile!=NULL&&!myLaser->getStatus()){
				myfile << "X "<<","<< "Y\n";
				myfile << *widthProfileX << "," << *widthProfileY <<","<< "limit"<<"\n";
				for(int i=0;i<frame.cols;i++){
					myfile << dataProfileX[i]<<","<< dataProfileY[i]<<"\n";
				}
				myfile.close();
				imwrite("./measurments/"+nomImage,frameT);     //Exporting the laser image to measurments
				myDisplay->addText(500,110,"Exported in /measurments");
			}
			else myDisplay->addText(500,110,"File not found");
			state = ST_TWOLIGHTPOINTS;
		}
	break; //end ST_STARTPAGE
	case ST_TWOLIGHTPOINTS:   //Pause mode: freeze display and export of the interface
			if(!calc_moment){    //test to calculate the moments only the first time (not at each refresh of the page)
				myLaser->calcDistanceX(&frameG);
				if(myLaser->getStatus()){	//if there was an error -> return to the startpage
					state = ST_STARTPAGE;
					break;
				}
				myLaser->calcDistanceY(&frameG);
				thresholdX=myLaser->getThresholdX();
				thresholdY=myLaser->getThresholdY();
				myLaser->findDistance(&frameG);
				resize(frameP1,frameP11,frameP11.size(),0,0,INTER_NEAREST);	//frameP1 to size of frameP11
			}
			circle(framec,myLaser->getMomentCent(0),myLaser->getRadius1(),Scalar(255,255,255),3);	//first center
			circle(framec,myLaser->getMomentCent(1),myLaser->getRadius2(),Scalar(255,255,255),3);	//second center
			resize(framec,frameAc,frameAc.size(),0,0,INTER_NEAREST);		//frameAc is the ouput image
			cvui::image(principalFrame,0,0,frameAc);     //Adding the frame to the interface
			if(display!=NORMAL){
			  resize(image,frameScale,frameScale.size(),0,0,INTER_NEAREST);
			  cvui::image(principalFrame,0,0,frameScale);	//add the image with the color scale to the frame
			}
			cvui::image(principalFrame,0,320,frameP11);		//PROFILE X
			myDisplay->addText(480,60,string("File: "+fileName+".png"));
			myDisplay->addText(480,80,string("App version: "+to_string(APP_VERSION).substr(0,3)));
			
			myDisplay->addText(480,120,("Beam delta:"+myLaser->getStrDistance()));
			myDisplay->addText(430,330,myCamera->getStrModel());
			myDisplay->addText(430,360,myCamera->getStrContrast());
			myDisplay->addText(430,380,myCamera->getStrSaturation());
			myDisplay->addText(430,400,myCamera->getStrLuminosity());
			myDisplay->addText(600,360,myCamera->getStrGain());
			myDisplay->addText(600,380,string("Smoothing="+to_string(filterPrecision)));
			myDisplay->addText(600,400,string("Shutter speed="+to_string(abs(myCamera->getExpoTime()*(100/99)*0.033)).substr(0,5)+"us"));
			myDisplay->addText(430,440,"Laser Beam Analyser by M.Geiser, A.Miollany, ");
			myDisplay->addText(430,460,"B.Dubuis & C.Tremblay");

			if(!calc_moment){
				imwrite("./measurments/"+fileName+".png",principalFrame);   //Once all the calculations are done, we export the frame
				calc_moment=true;
			}
			if(cvui::button(principalFrame,630,15,"About")){
				state=ST_ABOUT;          //It switches back to play mode every time the about mode is exited
				calc_moment=false;
			}
			if(cvui::button(principalFrame,440,15,"Play/Pause")) {
				state = ST_STARTPAGE;
				myCamera->setParameters();
				calc_moment=false;
			}
	break;
    case ST_ABOUT:           //button About pressed : display a text
    {
        if(cvui::button(principalFrame,40,15,"English")) language=ENGLISH;
        if(cvui::button(principalFrame,140,15,"Francais")) language=FRENCH;
        if(cvui::button(principalFrame,240,15,"Deutsch")) language=GERMAN;
        ifstream about_file;
        switch(language){
        case ENGLISH:
			about_file.open("about_english.txt");	//Opening a file for reading
		break;
        case FRENCH :
			about_file.open("about_francais.txt");  //Opening a file for reading
		break;
		case GERMAN:
			about_file.open("about_deutsch.txt");	//Opening a file for reading
        break;
        default:
			about_file.open("about_english.txt");	//Opening a file for reading
        break;
		}	//end switch
        if(about_file){   //required to open the file at each passage or else creation of a dynamic pointer of size = nb line
          string line;
          getline(about_file, line);   //No display of the first sentence of the file
          int initX=20;
          int initY=50;
          int step=20;   //Set the distance between text lines
          int k=0;
          while(getline(about_file, line)){
            cvui::printf(principalFrame,initX,initY+k*step,0.5,0xffffff,line.c_str());
            k++;
          }
          about_file.close();
		}else  cout << "ERROR: Unable to open the file for reading." << endl;
		if(cvui::button(principalFrame,630,15,"About")) state=ST_STARTPAGE;    //It switches back to play mode every time the about mode is exited
		if(cvui::button(principalFrame,440,15,"Play/Pause")) state = ST_STARTPAGE;
	break;
	}
	default:
			state=ST_STARTPAGE;
			break;
	}	//end switch

	if(cvui::button(principalFrame,720,15,"Quit")) break;
	cvui::update();
	cv::imshow(WINDOW_NAME, principalFrame);
	
	if (frame.empty()) {
		cerr << "ERROR! blank frame grabbed\n";
		break;
    }
    if (waitKey(16) >= 0) break;	// Quit when press Alt key on the keyboard
  }
}
