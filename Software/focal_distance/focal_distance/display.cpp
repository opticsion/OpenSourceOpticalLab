#include "display.hpp"
using namespace std;
using namespace cv;

/*======================================================================================
	Function name : Display(Mat* frame)
	return type :	-
	Constructor of the object display. Set the frame
	
======================================================================================*/
Display::Display(Mat* frame){
	displayFrame = frame;
}

/*======================================================================================
	Function name : addText
	return type :	-
	Display a text in a window from a string and a position
======================================================================================*/
void Display::addText(int posX, int posY, string text){
	cvui::printf(*displayFrame,posX,posY, 0.5, 0xffffff, "%s",text.c_str());
}

/*======================================================================================
	Function name : addText_size
	return type :	-
	Display a text in a window from a string and a position. select the text size
	
======================================================================================*/
void Display::addText_size(int posX, int posY, double size, string text){
	cvui::printf(*displayFrame,posX,posY, size, 0xffffff, "%s",text.c_str());
}

void Display::displayResult(){
	
}

/*===============================================================================
	Function name : add_cmp
	return type :	int
	Display function: add the buttons that manage the number of samples return the
	value of the filter precision.
================================================================================*/
int Display::add_cmp(int theX,int theY,int theWidth,int theHeight,int filterPrecision){ 
	addText(theX-30,theY-20,"Number of samples:(Smoothing)");
	rectangle(*displayFrame,Point(theX,theY),Point((theX+theWidth-1),(theY+theHeight-1)),Scalar(255,255,255),1); //Draw a rectangle around it
	if(cvui::button(*displayFrame, theX, theY,theWidth/6,theHeight, "|<")) filterPrecision=2;			//Back to minimum button
	if(cvui::button(*displayFrame, theX+theWidth/6, theY,theWidth/6,theHeight, "-")) filterPrecision--; //Decrease button
	addText(theX+2*theWidth/6,theY+4,to_string(filterPrecision));										//FilterPrecision counter
	if(cvui::button(*displayFrame, theX+4*theWidth/6, theY,theWidth/6,theHeight, "+")) filterPrecision++;    	//Increment button
	if(cvui::button(*displayFrame, theX+5*theWidth/6, theY,theWidth/6,theHeight, ">|")) filterPrecision=256;  	//Advance to maximum button
	return filterPrecision;
}

/*===============================================================================
	Function name : add_switch
	return type :	double
	Display function: add the switch to the desired position. Switch allows to
	choose the surface threshold between 13.5 and 50%. Display a switch with 2
	positions
================================================================================*/
double Display::add_switch(int theX,int theY,int theWidth,int theHeight,double limit){
	addText(theX-80,theY-20,"Threshold value:");
	rectangle(*displayFrame,Point(theX,theY),Point((theX+theWidth-1),(theY+theHeight-1)),Scalar(255,255,255),1); //Draw a rectangle around the switch
	addText(theX-55,theY+4,"13.5%");        //first choice on the left of the switch
	addText(theX+theWidth+5,theY+4,"50.0%");  		//second choice on the right of the switch
	if(limit==13.5){
		if(cvui::button(*displayFrame, theX, theY,2*theWidth/5,theHeight, "")) limit=50;    //The cursor is on the side of 13.5, if the user presses it, it goes to 50
	}else{
		if(cvui::button(*displayFrame, theX+3*theWidth/5, theY,2*theWidth/5,theHeight, "")) limit=13.5;  //Conversely
	}
	return limit; 	// Returns 13.5 or 50 depending on the location of the switch
					// 13.5% = 1/(e^2)
}
