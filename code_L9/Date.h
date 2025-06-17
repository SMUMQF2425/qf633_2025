#ifndef DATE_H
#define DATE_H

#include <string>
#include <chrono>

class Date {
private:
    int year_, month_, day_;
    
public:
    Date(); // Default constructor
    Date(int year, int month, int day);
    Date(const std::string& dateStr); // Format: YYYY-MM-DD
    
    // Operators
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
    bool operator>(const Date& other) const;
    
    // Date arithmetic
    Date addDays(int days) const;
    Date addMonths(int months) const;
    Date addYears(int years) const;
    
    // Utilities
    double yearFraction(const Date& other) const;
    int daysBetween(const Date& other) const;
    std::string toString() const;
    
    // Getters
    int getYear() const { return year_; }
    int getMonth() const { return month_; }
    int getDay() const { return day_; }
    
private:
    bool isLeapYear(int year) const;
    int daysInMonth(int year, int month) const;
    bool isValidDate(int year, int month, int day) const;
};

#endif