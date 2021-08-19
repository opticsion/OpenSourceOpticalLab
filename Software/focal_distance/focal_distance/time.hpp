#include <string>
#include <ctime>

class Time{
	private:
		int years;
		int months;
		int days;
		int seconds;
		int minutes;
		int hours;
	public:
		Time();
		std::string getTime();
};
