#include "time.hpp"
using namespace std;

#define INITYEAR 1900
Time::Time(){
  this->years=INITYEAR;
  this->months=0;
  this->days=0;
  this->seconds=0;
  this->minutes=0;
  this->hours=0;
}
/*======================================================================================
	Function name : getTime
	return type :	-
	Return the actual time in a string
======================================================================================*/
string Time::getTime(){
	string timeStr;
	
	time_t timer1;
	time(&timer1);
	
	struct tm *newTime1;
	newTime1 = localtime(&timer1); //acquisition of system time
			
	years = INITYEAR+newTime1->tm_year;
	months = 1+newTime1->tm_mon;
	days = newTime1->tm_mday;
	hours = newTime1->tm_hour;
	minutes = newTime1->tm_min;		
	seconds = newTime1->tm_sec;	
	timeStr=std::to_string(years)+"_"+std::to_string(months)+"_"+std::to_string(days)+"_"+std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(seconds);
	return timeStr;
}
