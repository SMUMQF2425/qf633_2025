#ifndef CURVE_H
#define CURVE_H

#include "Date.h"
#include <vector>
#include <map>
#include <memory>

class InterestRateCurve {
private:
    std::map<Date, double> curveData_;
    std::string curveName_;
    
public:
    InterestRateCurve(const std::string& curveName);
    
    void addPoint(const Date& date, double rate);
    double getZeroRate(const Date& date) const;
    double getDiscountFactor(const Date& date, const Date& baseDate) const;
    
    // Linear interpolation for missing dates
    double interpolateRate(const Date& date) const;
    
    void loadFromFile(const std::string& filename);
    
public:
    // Add these methods
    std::map<Date, double> getAllCurvePoints() const { return curveData_; }
    std::string getCurveName() const { return curveName_; }
    
private:
    Date parsetenor(const std::string& tenor, const Date& baseDate) const;
};

class VolatilityCurve {
private:
    std::map<Date, double> volData_;
    
public:
    VolatilityCurve();
    
    void addPoint(const Date& date, double vol);
    double getVolatility(const Date& date) const;
    double interpolateVol(const Date& date) const;
    
    void loadFromFile(const std::string& filename);
    
public:
    // Add this method
    std::map<Date, double> getAllVolPoints() const { return volData_; }
    
private:
    Date parseVolTenor(const std::string& tenor) const;
};

#endif