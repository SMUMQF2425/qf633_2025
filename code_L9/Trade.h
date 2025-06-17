#ifndef TRADE_H
#define TRADE_H

#include "Date.h"
#include "Curve.h"
#include <memory>
#include <string>
#include <map>

// Base Trade class
class Trade {
protected:
    int id_;
    std::string type_;
    Date tradeDate_;
    Date startDate_;
    Date endDate_;
    double notional_;
    std::string instrument_;
    
public:
    Trade(int id, const std::string& type, const Date& tradeDate,
          const Date& startDate, const Date& endDate, double notional,
          const std::string& instrument);
    
    virtual ~Trade() = default;
    
    virtual double calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                              const std::shared_ptr<VolatilityCurve>& volCurve,
                              const std::map<std::string, double>& stockPrices) const = 0;
    
    // Getters
    int getId() const { return id_; }
    std::string getType() const { return type_; }
    Date getEndDate() const { return endDate_; }
    double getNotional() const { return notional_; }
    
    virtual std::string getInstrument() const { return instrument_; }
};

// Swap class
class Swap : public Trade {
private:
    double fixedRate_;
    double frequency_;
    
public:
    Swap(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
         double notional, const std::string& instrument, double fixedRate, double frequency);
    
    double calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                      const std::shared_ptr<VolatilityCurve>& volCurve,
                      const std::map<std::string, double>& stockPrices) const override;
    
private:
    double calculateAnnuity(const std::shared_ptr<InterestRateCurve>& irCurve) const;
    double calculateParRate(const std::shared_ptr<InterestRateCurve>& irCurve) const;
};

// Bond class
class Bond : public Trade {
private:
    double couponRate_;
    double frequency_;
    
public:
    Bond(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
         double notional, const std::string& instrument, double couponRate, double frequency);
    
    double calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                      const std::shared_ptr<VolatilityCurve>& volCurve,
                      const std::map<std::string, double>& stockPrices) const override;
};

// Option base class
class Option : public Trade {
protected:
    double strike_;
    std::string optionType_; // "call" or "put"
    
public:
    Option(int id, const std::string& type, const Date& tradeDate, const Date& startDate,
           const Date& endDate, double notional, const std::string& instrument,
           double strike, const std::string& optionType);
    
    double getStrike() const { return strike_; }
    std::string getOptionType() const { return optionType_; }
    
protected:
    double blackScholesPrice(double spot, double strike, double timeToMaturity,
                           double riskFreeRate, double volatility, bool isCall) const;
    double normalCDF(double x) const;
};

// European Option
class EuropeanOption : public Option {
public:
    EuropeanOption(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
                   double notional, const std::string& instrument, double strike,
                   const std::string& optionType);
    
    double calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                      const std::shared_ptr<VolatilityCurve>& volCurve,
                      const std::map<std::string, double>& stockPrices) const override;
                      
private:
    double binomialTreePrice(double spot, double strike, double timeToMaturity,
                           double riskFreeRate, double volatility, bool isCall, int steps) const;
};

// American Option
class AmericanOption : public Option {
public:
    AmericanOption(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
                   double notional, const std::string& instrument, double strike,
                   const std::string& optionType);
    
    double calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                      const std::shared_ptr<VolatilityCurve>& volCurve,
                      const std::map<std::string, double>& stockPrices) const override;
                      
private:
    double americanBinomialTreePrice(double spot, double strike, double timeToMaturity,
                                   double riskFreeRate, double volatility, bool isCall, int steps) const;
};

#endif