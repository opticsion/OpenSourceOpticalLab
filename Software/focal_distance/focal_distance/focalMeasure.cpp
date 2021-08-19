#include "focalMeasure.hpp"
#include "cvplot.hpp"
using namespace std;
using namespace cv;

#define RESOLUTION_H 960
#define RESOLUTION_W 1280
#define CAM_RES_H 3040
#define CAM_RES_W 4056
#define MARGIN 10
/*======================================================================================
	Function name : LaserMeasure()
	return type :	-
	Constructor of the object LaserMeasure. Set the default values
======================================================================================*/
FocalMeasure::FocalMeasure(double pxSize){
	pixelSize = pxSize;
	error = false;
	beam[0]={
		.beamCenterX =0,
		.beamCenterY =0,
		.momentX =0,
		.momentY =0,
		.momentXY =0,
		.diamX =0,
		.diamY =0,
		.meanDiam =0,
	};
	beam[1]={
		.beamCenterX =0,
		.beamCenterY =0,
		.momentX =0,
		.momentY =0,
		.momentXY =0,
		.diamX =0,
		.diamY =0,
		.meanDiam =0,
	};
}

/*===============================================================================
	Function name : maximum
	return type :	Pixel*
	Searching for the maximum that will be recorded in a pixel and removing noise
	in the frame.
================================================================================*/
Pixel* FocalMeasure::maximum(Mat* frameG){
  std::vector<Pixel> v;
  int height=frameG->rows;
  int width=frameG->cols;
  int valMax=0;
  int valMin=255;
  
  // found the the pixel with the least brightness
  for (int x=0;x<width;x++){
		for(int y=0;y<height;y++){
			if (frameG->at<uchar>(y,x)<=valMin && frameG->at<uchar>(y,x)>0){
				min->setX(x);
				min->setY(y);
				valMin=frameG->at<uchar>(y,x);
		  }
	  }
  }
  min->setIntensity(valMin);	//valMin = noise

	//search the max
  for(int i=0;i<width;i++){
    for(int j=0;j<height;j++){
      if(frameG->at<uchar>(j,i)>=valMin) frameG->at<uchar>(j,i)-=valMin;    //Negative values that disturb the measurements are avoided
      else frameG->at<uchar>(j,i)=0;
      if (frameG->at<uchar>(j,i)>=valMax){    //The list is scanned and the new value is saved as soon as the temporary maximum is exceeded
        max->setX(i);
        max->setY(j);
        valMax=frameG->at<uchar>(j,i);        //frameG.at<uchar>(j,i) --> Access the element row j and column i in the frameG matrix
      }
	}
  }
  max->setIntensity(valMax);
  return max;
}

/*===============================================================================
	Function name : maxi
	return type :	Pixel*
	Searching for the maximum that will be recorded in a pixel and removing noise
	in the frame.
================================================================================*/
void FocalMeasure::maxi(Mat* frameG){
  std::vector<Pixel> v;
  int height=frameG->rows;
  int width=frameG->cols;
  int valMax=0;
  int valMin=255;

  // found the the pixel with the least brightness
  for (int x=0;x<width;x++){
		for(int y=0;y<height;y++){
			if (frameG->at<uchar>(y,x)<=valMin && frameG->at<uchar>(y,x)>0){
				min->setX(x);
				min->setY(y);
				valMin=frameG->at<uchar>(y,x);
		  }
	  }
  }
  min->setIntensity(valMin);	//valMin = noise

  //search the max
  for(int i=0;i<width;i++){
    for(int j=0;j<height;j++){
      if(frameG->at<uchar>(j,i)>=valMin) frameG->at<uchar>(j,i)-=valMin;    //Negative values that disturb the measurements are avoided
      else frameG->at<uchar>(j,i)=0;
      if (frameG->at<uchar>(j,i)>=valMax){    //The list is scanned and the new value is saved as soon as the temporary maximum is exceeded
        max->setCoord(i,j);
        valMax=frameG->at<uchar>(j,i);        //frameG.at<uchar>(j,i) --> Access the element row j and column i in the frameG matrix
      }
	}
  }
  max->setIntensity(valMax);
}

/*======================================================================================
	Function name : LaserMeasure()
	return type :	-
	Constructor of the object LaserMeasure. Set the default values.
======================================================================================*/
void FocalMeasure::moment(Mat* frameG, int beamNbr){
	float p=power(frameG);	// the denominator of equations
	int height=frameG->rows;
	int width=frameG->cols;
	
	
	for (int x=0;x<width;x++){
		for(int y=0;y<height;y++){
			beam[beamNbr].beamCenterX+=(x*frameG->at<uchar>(y,x));   // <char>(y,x)  
			beam[beamNbr].beamCenterY+=(y*frameG->at<uchar>(y,x));
		}
	}
	
	beam[beamNbr].beamCenterX=beam[beamNbr].beamCenterX/p;
	beam[beamNbr].beamCenterY=beam[beamNbr].beamCenterY/p;

	for (int x=0;x<width;x++){
		for(int y=0;y<height;y++){
			beam[beamNbr].momentX+=frameG->at<uchar>(y,x)*pow((x-beam[beamNbr].beamCenterX),2);
			beam[beamNbr].momentY+=frameG->at<uchar>(y,x)*pow((y-beam[beamNbr].beamCenterY),2);
			beam[beamNbr].momentXY+=frameG->at<uchar>(y,x)*(x-beam[beamNbr].beamCenterX)*(y-beam[beamNbr].beamCenterY);
		}
	}
	
	beam[beamNbr].momentX/=p;
	beam[beamNbr].momentY/=p;
	beam[beamNbr].momentXY/=p;
	
	beam[beamNbr].diamX=2*sqrt(2)*sqrt(beam[beamNbr].momentX+beam[beamNbr].momentY+2*abs(beam[beamNbr].momentXY)); //Value in pixel specific to the size of frameG
	beam[beamNbr].diamY=2*sqrt(2)*sqrt(beam[beamNbr].momentX+beam[beamNbr].momentY+2*abs(beam[beamNbr].momentXY)); //Value in pixel specific to the size of frameG
	beam[beamNbr].meanDiam=2*sqrt(2)*sqrt(beam[beamNbr].momentX+beam[beamNbr].momentY);  //Value in pixel unit
	
	//beam[beamNbr].beamCenterX*=pixelSize;	// convert value in um
	//beam[beamNbr].beamCenterY*=pixelSize;	// convert value in um
}

/*======================================================================================
	Function name : LaserMeasure()
	return type :	-
	Power calculation (relative to the filter): simple sum of all pixels in the image
	converted in grayscale.
======================================================================================*/
float FocalMeasure::power(Mat* frameG){
	int height=frameG->rows;
	int width=frameG->cols;
	float p=0;
	for (int i=0;i<width;i++){
		for(int j=0;j<height;j++){
			p+=frameG->at<uchar>(j,i);
		}
	}
	return p;
}

/*===============================================================================
	Function name : profileX
	return type :	Mat
	Create a figure that display the graphic with the max intensity on the row
======================================================================================*/
Mat FocalMeasure::profileX(Mat* frameG,Pixel* pixel,float* mean,float* standard_deviation,float* dataProfile,
  float* widthProfileX,double* limitWidth,int* filterPrecision, double pixSize){
  float normalizer=pixel->getIntensity();      // Normalization of values to the maximum pixel 
  float newVal=0.f;
  int width=frameG->cols;
  if(normalizer==0.f){
    normalizer=1.f;
  }
  int line=pixel->getY();
  float* array = (float*)malloc(frameG->cols*sizeof(float));
  float* arrayMean = (float*)malloc(frameG->cols*sizeof(float));
  for(int i=0;i<width;i++){
    array[i]=((double)frameG->at<uchar>(line,i))/normalizer;
    dataProfile[i]=array[i];
  }
  
  float variance=0;   // Calculation of mean, variance and standard deviation
  for(int j=0;j<width;j++){
    *mean+=array[j];
  }
  *mean=*mean/width;
  for(int j=0;j<width;j++){
    variance+=(array[j]-*mean)*(array[j]-*mean);
  }
  variance=variance/width;
  *standard_deviation=sqrt(variance);
  
  for(int i=0;i<width; i++){
  arrayMean[i]=0;
  }
  
  for(int i=*filterPrecision/2;i<width-(*filterPrecision/2)-1; i++){    //Smoothing the curve with filterPrecision
		newVal=0;
	for(int j=i-*filterPrecision/2;j<i+*filterPrecision/2;j++){
		newVal+=array[j];
	}
		newVal=newVal/(*filterPrecision);
		arrayMean[i]=newVal;
  }
  
  //Finding the max and min
  float maxTemp=0;
  float minTemp=1;
  int positionMax;
  for(int i=0;i<width;i++){
  	if(arrayMean[i]>maxTemp){
  		maxTemp=arrayMean[i];
  		positionMax=i;}
  	if(arrayMean[i]<minTemp){
  		minTemp=arrayMean[i];}
  }
  float limit=round((*limitWidth/100)*(maxTemp-minTemp)*100)/100;
  
  int positionX1=0;
  int positionX2=0;
  for(int i=0;i<positionMax;i++){
  	if(round(arrayMean[i]*100)/100==limit){
  		positionX1=i;
  	}
  }
  for(int i=positionMax;i<width;i++){
	if(round(arrayMean[i]*100)/100==limit){
  		positionX2=i;
	}
  }
  
  if(positionX1!=0 && positionX2!=0){
    *widthProfileX=pixSize*(positionX2-positionX1)*2;
  }
  
  CvPlot::Series serie= CvPlot::Series();
  serie.SetData(frameG->cols,array);
  
  CvPlot::Series meanSerie= CvPlot::Series();
  meanSerie.SetData(frameG->cols,arrayMean);
  
  CvPlot::Figure profileX = CvPlot::Figure("Profile X");
  profileX.Add(serie);
  profileX.Add(meanSerie);
  
  IplImage* p = profileX.Show();
  
  cv::Mat m = cv::cvarrToMat(p,true);
  cvReleaseImage(&p);
  
  return m;
}

/*===============================================================================
	Function name : profileY
	return type :	Mat
	Create a figure that display the graphic with the max intensity on the line
======================================================================================*/
Mat FocalMeasure::profileY(Mat* frameG,Pixel* max,float* mean,float* standard_deviation,float* dataProfile,
	float* widthProfileY,double* limitWidth,int* filterPrecision){
  float normalizer=max->getIntensity();
  int width=frameG->rows;
  float newVal;
  if(normalizer==0.f){
    normalizer=1.f;
  }
  int colonne=max->getX();
  float* array = (float*)malloc(frameG->rows*sizeof(float));
  float* arrayMean = (float*)malloc(frameG->rows*sizeof(float));
  for(int i=0;i<frameG->rows;i++){
    array[i]=((double)frameG->at<uchar>(i,colonne))/normalizer;
    dataProfile[i]=array[i];
  }

  float variance=0;
  for(int j=0;j<width;j++){
    *mean+=array[j];
  }
  *mean=*mean/width;
  for(int j=0;j<width;j++){
    variance+=(array[j]-*mean)*(array[j]-*mean);
  }
  variance=variance/width;
  *standard_deviation=sqrt(variance);

  for(int i=0;i<width; i++){
    arrayMean[i]=0;
  }

	for(int i=*filterPrecision/2;i<width-(*filterPrecision/2)-1; i++){
	  newVal=0;
		for(int j=i-*filterPrecision/2;j<i+*filterPrecision/2;j++){
			newVal+=array[j];
		}
    newVal=newVal/(*filterPrecision);
		arrayMean[i]=newVal;
	}

	//Finding the max and min
	float maxTemp=0;
	float minTemp=1;
	int positionMax;
	for(int i=0;i<width;i++){
	  if(arrayMean[i]>maxTemp){
			maxTemp=arrayMean[i];
			positionMax=i;
    }
		if(arrayMean[i]<minTemp){
			minTemp=arrayMean[i];
    }
	}
	float limit=round((*limitWidth/100)*(maxTemp-minTemp)*100)/100;
	
	int positionX1=0;
	int positionX2=0;
	for(int i=0;i<positionMax;i++){
		if(round(arrayMean[i]*100)/100==limit){
			positionX1=i;
		}
	}
	for(int i=positionMax;i<width;i++){
		if(round(arrayMean[i]*100)/100==limit){
			positionX2=i;
		}
	}

	if(positionX1!=0 && positionX2!=0){
		*widthProfileY=pixelSize*(positionX2-positionX1)*2;
	}

  CvPlot::Series serie= CvPlot::Series();
  serie.SetData(frameG->rows,array);
  CvPlot::Series serieMean= CvPlot::Series();
  serieMean.SetData(frameG->rows,arrayMean);
  CvPlot::Figure profileY = CvPlot::Figure("Profile Y");
  profileY.Add(serie);
  profileY.Add(serieMean);

	IplImage* p = profileY.Show();

	cv::Mat m = cv::cvarrToMat(p,true);
    cvReleaseImage(&p);

  return m;    
}

/*===============================================================================
	Function name : calcDistanceX
	return type :	void
	found the localisation of the lasers beam and store the X outline
======================================================================================*/
void FocalMeasure::calcDistanceX(Mat* frameG){
	Mat binFrame = Mat(frameG->rows,frameG->cols,CV_8UC3);
	
	int height = frameG->rows;
	int width = frameG->cols;
	bool profileX1D [frameG->cols+2];
	profileX1D[frameG->cols+1]=false;
	profileX1D[frameG->cols]=false;
	max = maximum(frameG);
	int foundThreshold = 0;
	bool error = false;
	for(int x = 0; x<width; x++){
		profileX1D[x]=false;	//init value
		for(int y = 0; y<height; y++){
			if(frameG->at<uchar>(y,x)>(max->getIntensity()/2)){
				binFrame.at<uchar>(y,x)=255;
				profileX1D[x] = true;
			}
			else binFrame.at<uchar>(y,x)=0;
		}
	}

	//check that there are several columns in a row that are above the threshold
	for(int x = 0; x<width; x++){
		if(profileX1D[x]!=profileX1D[x+1]&&profileX1D[x+1]==profileX1D[x+2]){
				if(foundThreshold>3) error = true;
				thresholdX[foundThreshold%4]=x+1;
				foundThreshold++;
		}
	}
}

/*===============================================================================
	Function name : calcDistanceY
	return type :	void
	found the localisation of the lasers beam and store the Y outline
======================================================================================*/
void FocalMeasure::calcDistanceY(Mat* frameG){
	Mat binFrame= Mat(frameG->rows,frameG->cols,CV_8UC3);
	
	int height = frameG->rows;
	int width = frameG->cols;
	
	bool profileY1D [frameG->rows+2];
	profileY1D[frameG->rows+1]=false;
	profileY1D[frameG->rows]=false;
	max = maximum(frameG);
	int foundThreshold = 0;
	bool error = false;
	for(int y = 0; y<height; y++){
		profileY1D[y]=false;	//init value
		for(int x = 0; x<height; x++){
			if(frameG->at<uchar>(y,x)>(max->getIntensity()/2)){
				binFrame.at<uchar>(y,x)=255;
				profileY1D[y] = true;
			}
			else binFrame.at<uchar>(y,x)=0;
		}
	}
	
	for(int y = 0; y<height; y++){
		if(profileY1D[y]!=profileY1D[y+1]&&profileY1D[y+1]==profileY1D[y+3]){
				if(foundThreshold>2) error = true;
				thresholdY[foundThreshold%2]=y+1;
				foundThreshold++;
		}
	}
	
	
	centerY = (thresholdY[0]+thresholdY[1])/2;
}

/*===============================================================================
	Function name : calcDistanceY
	return type :	void
	The first step is to crop 2 times the grayscale frame. The goal is to obtain an 
	image with one
======================================================================================*/
void FocalMeasure::findDistance(Mat* frameG){
	Mat image_copy = frameG->clone();
	int rangeX1 = thresholdX[0]-MARGIN;
	int rangeX2 = thresholdX[2]-MARGIN;
	
	Mat cropped1 = image_copy(Range(thresholdY[0]-MARGIN, thresholdY[1]+MARGIN), Range(thresholdX[0]-MARGIN,thresholdX[1]+MARGIN));
	Mat cropped2 = image_copy(Range(thresholdY[0]-MARGIN, thresholdY[1]+MARGIN), Range(thresholdX[2]-MARGIN,thresholdX[3]+MARGIN));
	
	moment(&cropped1, 0);
	moment(&cropped2, 1);
	distance = sqrt(pow((abs(beam[0].beamCenterX-beam[1].beamCenterX)+abs(rangeX1-rangeX2)),2)+pow(beam[0].beamCenterY-beam[1].beamCenterY,2));
}

/*===============================================================================
	Function name : getMax
	return type :	Pixel*
	return the position of the maximum
======================================================================================*/
Pixel* FocalMeasure::getMax(){
	return max;
}

/*===============================================================================
	Function name : getRadius1
	return type :	int
	return the radius of the first beam
=================================================================================*/
int FocalMeasure::getRadius1(){
	return int(beam[0].meanDiam/2);
}

/*===============================================================================
	Function name : getRadius2
	return type :	int
	return the radius of the second beam
=================================================================================*/
int FocalMeasure::getRadius2(){
	return int(beam[1].meanDiam/2);
}

/*===============================================================================
	Function name : strCentX
	return type :	string
	return the str of the first center in X
=================================================================================*/
string FocalMeasure::strCentX(){
	return to_string(abs(center1-center2)*pixelSize*4056/1280).substr(0,6)+"um";
}

/*===============================================================================
	Function name : strCentY
	return type :	string
	return the str of the first center in Y
=================================================================================*/
string FocalMeasure::strCentY(){
	return to_string(beam[0].beamCenterY*pixelSize*CAM_RES_W/RESOLUTION_W).substr(0,6)+"um";
}

/*===============================================================================
	Function name : getThresholdX
	return type :	int
	return the first position of the threshold array
=================================================================================*/
int* FocalMeasure::getThresholdX(){
	return thresholdX;
}

/*===============================================================================
	Function name : getThresholdY
	return type :	int
	return the first position of the threshold array
=================================================================================*/
int* FocalMeasure::getThresholdY(){
	return thresholdY;
}

/*===============================================================================
	Function name : getStatus
	return type :	int
	Return if there was an error in calculation
=================================================================================*/
bool FocalMeasure::getStatus(){
	return error;
}

/*===============================================================================
	Function name : getMomentCent
	return type :	int
	Return the center of a beam.
=================================================================================*/
Point FocalMeasure::getMomentCent(int nb){
	return Point(beam[nb].beamCenterX+thresholdX[nb*2]-MARGIN, beam[nb].beamCenterY+thresholdY[0]-MARGIN);
}

/*===============================================================================
	Function name : getDistance
	return type :	int
	Return the center of a beam.
=================================================================================*/
string FocalMeasure::getStrDistance(){
	return to_string(distance*pixelSize*CAM_RES_W/RESOLUTION_W).substr(0,6)+"um";
}
