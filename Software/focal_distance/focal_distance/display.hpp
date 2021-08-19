#include <string.h>
#include <string>
#include "cvui.h"


class Display{
	private:
		cv::Mat* displayFrame;
	public:
		Display(cv::Mat* frame);
		void addText(int posX, int posY, std::string text);
		void addText_size(int posX, int posY, double size, std::string text);
		void displayResult();
		int add_cmp( int theX,int theY,int theWidth,int theHeight,int filterPrecision);
		double add_switch(int theX,int theY,int theWidth,int theHeight,double limit);
};
