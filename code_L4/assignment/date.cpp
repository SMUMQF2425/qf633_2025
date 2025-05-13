#include "date.h"
#include <string>

Date::Date(const std::string &s)
{
	year = std::stoi(s.substr(0, 4));
	month = std::stoi(s.substr(5, 2));
	day = std::stoi(s.substr(8, 2));
}

int Date::operator-(const Date &dt2) const
{
	int dy = dt2.year - year;
	int dm = dt2.month - month;
	int dd = dt2.day - day;
	return dy * 365 + dm * 30 + dd;
}