#pragma once
#include <string>

class Date
{
public:
	Date(const std::string &dateStr);
	int operator-(const Date &dt2) const;

private:
	int year, month, day;
};