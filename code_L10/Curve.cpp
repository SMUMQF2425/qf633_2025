#include "Curve.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <cmath>

InterestRateCurve::InterestRateCurve(const std::string& curveName) : curveName_(curveName) {}

void InterestRateCurve::addPoint(const Date& date, double rate) {
    curveData_[date] = rate;
}

double InterestRateCurve::getZeroRate(const Date& date) const {
    auto it = curveData_.find(date);
    if (it != curveData_.end()) {
        return it->second;
    }
    return interpolateRate(date);
}

double InterestRateCurve::getDiscountFactor(const Date& date, const Date& baseDate) const {
    double zeroRate = getZeroRate(date);
    double timeToMaturity = baseDate.yearFraction(date);
    return std::exp(-zeroRate * timeToMaturity);
}

double InterestRateCurve::interpolateRate(const Date& date) const {
    if (curveData_.empty()) {
        throw std::runtime_error("No data points in curve");
    }
    
    // Find the two closest points for linear interpolation
    auto upper = curveData_.upper_bound(date);
    if (upper == curveData_.begin()) {
        return curveData_.begin()->second; // Extrapolate flat from first point
    }
    if (upper == curveData_.end()) {
        return curveData_.rbegin()->second; // Extrapolate flat from last point
    }
    
    auto lower = std::prev(upper);
    
    // Linear interpolation
    double t1 = Date(2025, 1, 1).yearFraction(lower->first);
    double t2 = Date(2025, 1, 1).yearFraction(upper->first);
    double t = Date(2025, 1, 1).yearFraction(date);
    
    double rate1 = lower->second;
    double rate2 = upper->second;
    
    return rate1 + (rate2 - rate1) * (t - t1) / (t2 - t1);
}

void InterestRateCurve::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    std::getline(file, line); // Skip header
    
    Date baseDate(2025, 1, 1);
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string tenor, rateStr;
        
        std::getline(iss, tenor, ':');
        std::getline(iss, rateStr);
        
        // Remove whitespace and % sign
        tenor.erase(std::remove_if(tenor.begin(), tenor.end(), ::isspace), tenor.end());
        rateStr.erase(std::remove_if(rateStr.begin(), rateStr.end(), ::isspace), rateStr.end());
        rateStr.erase(std::remove(rateStr.begin(), rateStr.end(), '%'), rateStr.end());
        
        Date maturityDate = parsetenor(tenor, baseDate);
        double rate = std::stod(rateStr) / 100.0; // Convert percentage to decimal
        
        addPoint(maturityDate, rate);
    }
}

Date InterestRateCurve::parsetenor(const std::string& tenor, const Date& baseDate) const {
    if (tenor == "ON") return baseDate.addDays(1);
    
    char unit = tenor.back();
    int value = std::stoi(tenor.substr(0, tenor.length() - 1));
    
    switch (unit) {
        case 'M': return baseDate.addMonths(value);
        case 'Y': return baseDate.addYears(value);
        default: throw std::invalid_argument("Unknown tenor: " + tenor);
    }
}

// VolatilityCurve implementation
VolatilityCurve::VolatilityCurve() {}

void VolatilityCurve::addPoint(const Date& date, double vol) {
    volData_[date] = vol;
}

double VolatilityCurve::getVolatility(const Date& date) const {
    auto it = volData_.find(date);
    if (it != volData_.end()) {
        return it->second;
    }
    return interpolateVol(date);
}

double VolatilityCurve::interpolateVol(const Date& date) const {
    if (volData_.empty()) {
        throw std::runtime_error("No volatility data points");
    }
    
    // Similar interpolation logic as interest rate curve
    auto upper = volData_.upper_bound(date);
    if (upper == volData_.begin()) {
        return volData_.begin()->second;
    }
    if (upper == volData_.end()) {
        return volData_.rbegin()->second;
    }
    
    auto lower = std::prev(upper);
    
    double t1 = Date(2025, 1, 1).yearFraction(lower->first);
    double t2 = Date(2025, 1, 1).yearFraction(upper->first);
    double t = Date(2025, 1, 1).yearFraction(date);
    
    double vol1 = lower->second;
    double vol2 = upper->second;
    
    return vol1 + (vol2 - vol1) * (t - t1) / (t2 - t1);
}

void VolatilityCurve::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::string line;
    Date baseDate(2025, 1, 1);
    
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string tenor, volStr;
        
        std::getline(iss, tenor, ':');
        std::getline(iss, volStr);
        
        // Clean up strings
        tenor.erase(std::remove_if(tenor.begin(), tenor.end(), ::isspace), tenor.end());
        volStr.erase(std::remove_if(volStr.begin(), volStr.end(), ::isspace), volStr.end());
        volStr.erase(std::remove(volStr.begin(), volStr.end(), '%'), volStr.end());
        
        Date maturityDate = parseVolTenor(tenor);
        double vol = std::stod(volStr) / 100.0;
        
        addPoint(maturityDate, vol);
    }
}

Date VolatilityCurve::parseVolTenor(const std::string& tenor) const {
    Date baseDate(2025, 1, 1);
    
    char unit = tenor.back();
    int value = std::stoi(tenor.substr(0, tenor.length() - 1));
    
    switch (unit) {
        case 'M': return baseDate.addMonths(value);
        case 'Y': return baseDate.addYears(value);
        default: throw std::invalid_argument("Unknown vol tenor: " + tenor);
    }
}