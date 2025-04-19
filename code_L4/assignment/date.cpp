#include <cmath>
#include "date.h"

Date::Date(const string& dateStr) 
{
	year = stoi(dateStr.substr(0, 4));
	month = stoi(dateStr.substr(5, 2));
	day = stoi(dateStr.substr(8, 2));
}

// return number of days between 2 dates object
int Date::operator-(const Date& dt2) {
	int numOfYs = dt2.year - this->year;
	int numOfMs = dt2.month - this->month;
	int numOfDs = dt2.day - this->day;
	return numOfYs * 365 + numOfMs * 30 + numOfDs;
}