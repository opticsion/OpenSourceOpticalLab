#include "camera.hpp"
using namespace std;
using namespace cv;


/*======================================================================================
	Function name : Camera()
	return type :	-
	Constructor of the object camera. Set the default values
======================================================================================*/
Camera::Camera(raspicam::RaspiCam_Cv* camera){
		pixelSize = 1.55;
		model="Pi HQ Camera";
		luminosity=50;
		saturation=50;
		contrast=50;
		gain=50;
		expoTime=-1;
		myCapture = *camera;
}

/*======================================================================================
	Function name : setParameters()
	return type :	-
	Set the parameters of the camera
======================================================================================*/
void Camera::setParameters(){
	myCapture.set(CV_CAP_PROP_EXPOSURE,expoTime);   //Update of the camera settings at every turn
	myCapture.set(CV_CAP_PROP_CONTRAST ,contrast);
	myCapture.set(CV_CAP_PROP_SATURATION, saturation);
	myCapture.set(CV_CAP_PROP_GAIN, gain);
	myCapture.set(CV_CAP_PROP_BRIGHTNESS,luminosity);
//	myCapture.set(CV_CAP_PROP_FRAME_WIDTH, 4056);
//	myCapture.set(CV_CAP_PROP_FRAME_HEIGHT,3040);
}

/*======================================================================================
	Function name : setContrast()
	return type :	-
	Set the contrast value
======================================================================================*/
void Camera::setContrast(int cont){
	this->contrast=cont;
}
/*======================================================================================
	Function name : ()
	return type :	-
	Set the luminosity value
======================================================================================*/
void Camera::setLuminosity(int lum){
	this->luminosity = lum;
}
/*======================================================================================
	Function name : setSaturation()
	return type :	-
	Set the saturation value
======================================================================================*/
void Camera::setSaturation(int sat){
	this->saturation=sat;
}
/*======================================================================================
	Function name : setGain()
	return type :	-
	Set the value
======================================================================================*/
void Camera::setGain(int gain){
	this->gain = gain;
}
/*======================================================================================
	Function name : setExpoTime()
	return type :	-
	Set the expo value
======================================================================================*/
void Camera::setExpoTime(int expT){
	this->expoTime = expT;
}

/*======================================================================================
	Function name : getPixelSize()
	return type :	int
	Return the size of pixel size
======================================================================================*/
double Camera::getPixelSize(){
	return this->pixelSize;
}

/*======================================================================================
	Function name : getLuminosity()
	return type :	int
	Return the luminosity value
======================================================================================*/
int Camera::getLuminosity(){
	return this->luminosity;
}
/*======================================================================================
	Function name : getSaturation()
	return type :	int
	Return the saturation value
======================================================================================*/
int Camera::getSaturation(){
	return this->saturation;
}
/*======================================================================================
	Function name : getContrast()
	return type :	int
	Return the contrast value
======================================================================================*/
int Camera::getContrast(){
	return this->contrast;
}
/*======================================================================================
	Function name : getGain()
	return type :	int
	Return the gain value
======================================================================================*/
int Camera::getGain(){
	return this->gain;
}
/*======================================================================================
	Function name : getExpoTime()
	return type :	int
	Return the time of exposition when a picture is captured
======================================================================================*/
int Camera::getExpoTime(){
	return this->expoTime;
}
/*======================================================================================
	Function name : getStrModel()
	return type :	string
	Return the model value
======================================================================================*/
string Camera::getStrModel(){
	return string("Camera features: "+this->model);
}

/*======================================================================================
	Function name : getStrLuminosity()
	return type :	string
	Return the luminosity value
======================================================================================*/
string Camera::getStrLuminosity(){
	return string("Luminosity="+to_string(this->luminosity));
}

/*======================================================================================
	Function name : getStrSatuation()
	return type :	string
	Return the saturation value
======================================================================================*/
string Camera::getStrSaturation(){
	return string("Saturation="+to_string(this->saturation));
}

/*======================================================================================
	Function name : getStrContrast()
	return type :	string
	Return the contrast value
======================================================================================*/
string Camera::getStrContrast(){
	return string("Contrast="+to_string(this->contrast));
}

/*======================================================================================
	Function name : getStrGain()
	return type :	string
	Return the Gain value
======================================================================================*/

string Camera::getStrGain(){
	return string("Gain="+to_string(this->gain));
}

/*======================================================================================
	Function name : getStrExpoTime()
	return type :	string
	Return the expoTime value
======================================================================================*/
string Camera::getStrExpoTime(){
	return to_string(this->expoTime);
}

/*======================================================================================
	Function name : grabImage()
	return type :	Mat
	Return the picture taken
======================================================================================*/
void Camera::grabImage(){
	/*myCapture.grab();				// wait for a new frame from camera and store it into 'frame'
	cout<<"grab done"<< endl;
	myCapture.retrieve(frameG);	//Capturing the image
	cout<<"retrieve done" << endl;
	return frameG;*/
}

