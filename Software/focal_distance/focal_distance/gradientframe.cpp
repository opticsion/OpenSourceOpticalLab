#include "gradientframe.hpp"
//using namespace cv;

/*======================================================================================
	Function name : Frame()
	return type :	-
	Constructor of the object display. Set the frame
	
======================================================================================*/
GradientFrame::GradientFrame(){
}

/*===============================================================================
	Function name : gradient_relative
	return type :	void
	Assigns a colour to the pixel according to its light intensity. The objetive
	is to construct an image that has the most easily readable light intensity
	for the user. The scale of color is choosen in function of the max intensity
================================================================================*/
void GradientFrame::gradient_relative(int value,int* rgb,int max){
    max++;
    int step=int(max/5);
    if(value<1){
      rgb[0]=0;
      rgb[1]=0;
      rgb[2]=0;
    }
    else if(value<=step){
      rgb[0]=255-int(255/step*value);
      rgb[1]=0;
      rgb[2]=255;
      }
    else if(value<=2*step){
      rgb[0]=0;
      rgb[1]=-255+int(255/step*value);
      rgb[2]=255;
    }
    else if(value<=3*step){
      rgb[0]=0;
      rgb[1]=255;
      rgb[2]=255*3-int(255/step*value);
    }
    else if(value<=4*step){
      rgb[0]=-255*3+int(255/step*value);
      rgb[1]=255;
      rgb[2]=0;
    }
    else{
      rgb[0]=255;
      rgb[1]=255*5-int(255/step*value);
      rgb[2]=0;
    }
}

/*===============================================================================
	Function name : gradient_absolute
	return type :	void
	Assigns a colour to the pixel according to its light intensity. The objetive
	is to construct an image that has the most easily readable light intensity
	for the user. The scale of color is choosen without taking importance of the
	max intensity
================================================================================*/
void GradientFrame::gradient_absolute(int value,int* rgb){
  int step=51;
    if(value<1){
      rgb[0]=0;
      rgb[1]=0;
      rgb[2]=0;
    }
    else if(value<=step){
      rgb[0]=255-int(255/step*value);
      rgb[1]=0;
      rgb[2]=255;
      }
    else if(value<=2*step){
      rgb[0]=0;
      rgb[1]=-255+int(255/step*value);
      rgb[2]=255;
    }
    else if(value<=3*step){
      rgb[0]=0;
      rgb[1]=255;
      rgb[2]=255*3-int(255/step*value);
    }
    else if(value<=4*step){
      rgb[0]=-255*3+int(255/step*value);
      rgb[1]=255;
      rgb[2]=0;
    }
    else{
      rgb[0]=255;
      rgb[1]=255*5-int(255/step*value);
      rgb[2]=0;
    }
}
