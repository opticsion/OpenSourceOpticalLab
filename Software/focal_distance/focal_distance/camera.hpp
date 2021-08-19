#include <string>
#include <raspicam/raspicam_cv.h>
#include <iostream>

class Camera{
	private:
		double pixelSize;
		std::string model;
		int luminosity;
		int saturation;
		int contrast;
		int gain;
		int expoTime;
		raspicam::RaspiCam_Cv myCapture;
	public:
		Camera(raspicam::RaspiCam_Cv* camera);
		void setParameters();
		void setLuminosity(int lum);
		void setSaturation(int sat);
		void setContrast(int cont);
		void setGain(int gain);
		void setExpoTime(int expT);
		double getPixelSize();
		int getLuminosity();
		int getSaturation();
		int getContrast();
		int getGain();
		int getExpoTime();
		std::string getStrModel();
		std::string getStrLuminosity();
		std::string getStrSaturation();
		std::string getStrContrast();
		std::string getStrGain();
		std::string getStrExpoTime();
		void grabImage();
};
