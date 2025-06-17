#include "Date.h"
#include <sstream>
#include <stdexcept>
#include <iomanip>

Date::Date(int year, int month, int day) : year_(year), month_(month), day_(day) {
    if (!isValidDate(year, month, day)) {
        throw std::invalid_argument("Invalid date");
    }
}

Date::Date(const std::string& dateStr) {
    std::istringstream iss(dateStr);
    std::string token;
    
    std::getline(iss, token, '-');
    year_ = std::stoi(token);
    std::getline(iss, token, '-');
    month_ = std::stoi(token);
    std::getline(iss, token, '-');
    day_ = std::stoi(token);
    
    if (!isValidDate(year_, month_, day_)) {
        throw std::invalid_argument("Invalid date string: " + dateStr);
    }
}

Date::Date() : year_(2025), month_(1), day_(1) {
    // Default to Jan 1, 2025
}

bool Date::operator<(const Date& other) const {
    if (year_ != other.year_) return year_ < other.year_;
    if (month_ != other.month_) return month_ < other.month_;
    return day_ < other.day_;
}

bool Date::operator==(const Date& other) const {
    return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}

bool Date::operator>(const Date& other) const {
    return !(*this < other) && !(*this == other);
}

Date Date::addDays(int days) const {
    // Handle edge cases
    if (days == 0) return *this;
    
    int newYear = year_;
    int newMonth = month_;
    int newDay = day_;
    
    if (days > 0) {
        // Adding days
        newDay += days;
        
        while (newDay > daysInMonth(newYear, newMonth)) {
            newDay -= daysInMonth(newYear, newMonth);
            newMonth++;
            
            if (newMonth > 12) {
                newMonth = 1;
                newYear++;
            }
        }
    } else {
        // Subtracting days
        newDay += days; // days is negative
        
        while (newDay <= 0) {
            newMonth--;
            
            if (newMonth <= 0) {
                newMonth = 12;
                newYear--;
            }
            
            newDay += daysInMonth(newYear, newMonth);
        }
    }
    
    return Date(newYear, newMonth, newDay);
}

Date Date::addMonths(int months) const {
    int newMonth = month_ + months;
    int newYear = year_;
    
    while (newMonth > 12) {
        newMonth -= 12;
        newYear++;
    }
    while (newMonth < 1) {
        newMonth += 12;
        newYear--;
    }
    
    int newDay = std::min(day_, daysInMonth(newYear, newMonth));
    return Date(newYear, newMonth, newDay);
}

Date Date::addYears(int years) const {
    return Date(year_ + years, month_, day_);
}

double Date::yearFraction(const Date& other) const {
    return daysBetween(other) / 365.25;
}

int Date::daysBetween(const Date& other) const {
    std::tm tm1 = {0, 0, 0, day_, month_ - 1, year_ - 1900};
    std::tm tm2 = {0, 0, 0, other.day_, other.month_ - 1, other.year_ - 1900};
    
    std::time_t time1 = std::mktime(&tm1);
    std::time_t time2 = std::mktime(&tm2);
    
    return static_cast<int>(std::difftime(time2, time1) / (60 * 60 * 24));
}

std::string Date::toString() const {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(4) << year_ << "-"
        << std::setw(2) << month_ << "-" << std::setw(2) << day_;
    return oss.str();
}

bool Date::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Date::daysInMonth(int year, int month) const {
    const int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) return 29;
    return days[month - 1];
}

bool Date::isValidDate(int year, int month, int day) const {
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > daysInMonth(year, month)) return false;
    return true;
}