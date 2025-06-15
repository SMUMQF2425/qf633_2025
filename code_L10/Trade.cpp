#include "Trade.h"
#include <cmath>
#include <algorithm>

// Trade base class
Trade::Trade(int id, const std::string& type, const Date& tradeDate,
             const Date& startDate, const Date& endDate, double notional,
             const std::string& instrument)
    : id_(id), type_(type), tradeDate_(tradeDate), startDate_(startDate),
      endDate_(endDate), notional_(notional), instrument_(instrument) {}

// Swap implementation
Swap::Swap(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
           double notional, const std::string& instrument, double fixedRate, double frequency)
    : Trade(id, "swap", tradeDate, startDate, endDate, notional, instrument),
      fixedRate_(fixedRate), frequency_(frequency) {}

double Swap::calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                        const std::shared_ptr<VolatilityCurve>& volCurve,
                        const std::map<std::string, double>& stockPrices) const {
    
    double annuity = calculateAnnuity(irCurve);
    double parRate = calculateParRate(irCurve);
    
    // PV = (Fixed Rate - Par Rate) * Annuity
    return (fixedRate_ - parRate) * annuity;
}

double Swap::calculateAnnuity(const std::shared_ptr<InterestRateCurve>& irCurve) const {
    double annuity = 0.0;
    Date currentDate = startDate_;
    
    while (currentDate < endDate_) {
        Date nextDate = currentDate.addMonths(static_cast<int>(12 / frequency_));
        if (nextDate > endDate_) nextDate = endDate_;
        
        double periodLength = currentDate.yearFraction(nextDate);
        double discountFactor = irCurve->getDiscountFactor(nextDate, Date(2025, 1, 1));
        
        annuity += periodLength * discountFactor;
        currentDate = nextDate;
    }
    
    return notional_ * annuity;
}

double Swap::calculateParRate(const std::shared_ptr<InterestRateCurve>& irCurve) const {
    // Par rate = (1 - Final DF) / Annuity
    // Where Final DF is the discount factor at swap maturity
    // And Annuity is the sum of period DFs weighted by period length
    
    Date baseDate(2025, 1, 1);
    double finalDF = irCurve->getDiscountFactor(endDate_, baseDate);
    
    // Calculate the annuity (sum of discounted periods)
    double annuity = 0.0;
    Date currentDate = startDate_;
    
    while (currentDate < endDate_) {
        Date nextDate;
        
        // Calculate next coupon date based on frequency
        if (frequency_ == 0.25) {
            nextDate = currentDate.addMonths(3); // Quarterly
        } else if (frequency_ == 0.5) {
            nextDate = currentDate.addMonths(6); // Semi-annual
        } else if (frequency_ == 1.0) {
            nextDate = currentDate.addYears(1); // Annual
        } else {
            // For custom frequencies, convert to months
            int monthsToAdd = static_cast<int>(12.0 / frequency_);
            nextDate = currentDate.addMonths(monthsToAdd);
        }
        
        // Ensure we don't go past the end date
        if (nextDate > endDate_) {
            nextDate = endDate_;
        }
        
        // Calculate period length in years
        double periodLength = currentDate.yearFraction(nextDate);
        
        // Get discount factor for this payment date
        double df = irCurve->getDiscountFactor(nextDate, baseDate);
        
        // Add to annuity
        annuity += periodLength * df;
        
        currentDate = nextDate;
    }
    
    // Avoid division by zero
    if (annuity == 0.0) {
        throw std::runtime_error("Cannot calculate par rate: annuity is zero");
    }
    
    // Par rate formula: (1 - Final DF) / Annuity
    double parRate = (1.0 - finalDF) / annuity;
    
    return parRate;
}

// Bond implementation
Bond::Bond(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
           double notional, const std::string& instrument, double couponRate, double frequency)
    : Trade(id, "bond", tradeDate, startDate, endDate, notional, instrument),
      couponRate_(couponRate), frequency_(frequency) {}

double Bond::calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                        const std::shared_ptr<VolatilityCurve>& volCurve,
                        const std::map<std::string, double>& stockPrices) const {
    
    double pv = 0.0;
    Date currentDate = startDate_;
    
    // Discount coupon payments
    while (currentDate < endDate_) {
        Date nextDate = currentDate.addMonths(static_cast<int>(12 / frequency_));
        if (nextDate > endDate_) nextDate = endDate_;
        
        double couponAmount = (couponRate_ / frequency_) * notional_;
        double discountFactor = irCurve->getDiscountFactor(nextDate, Date(2025, 1, 1));
        pv += couponAmount * discountFactor;
        
        currentDate = nextDate;
    }
    
    // Discount principal repayment
    double principalDF = irCurve->getDiscountFactor(endDate_, Date(2025, 1, 1));
    pv += notional_ * principalDF;
    
    return pv;
}

// Option base class
Option::Option(int id, const std::string& type, const Date& tradeDate, const Date& startDate,
               const Date& endDate, double notional, const std::string& instrument,
               double strike, const std::string& optionType)
    : Trade(id, type, tradeDate, startDate, endDate, notional, instrument),
      strike_(strike), optionType_(optionType) {}

double Option::blackScholesPrice(double spot, double strike, double timeToMaturity,
                                double riskFreeRate, double volatility, bool isCall) const {
    
    if (timeToMaturity <= 0) return std::max(0.0, isCall ? spot - strike : strike - spot);
    
    double d1 = (std::log(spot / strike) + (riskFreeRate + 0.5 * volatility * volatility) * timeToMaturity) 
                / (volatility * std::sqrt(timeToMaturity));
    double d2 = d1 - volatility * std::sqrt(timeToMaturity);
    
    if (isCall) {
        return spot * normalCDF(d1) - strike * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(d2);
    } else {
        return strike * std::exp(-riskFreeRate * timeToMaturity) * normalCDF(-d2) - spot * normalCDF(-d1);
    }
}

double Option::normalCDF(double x) const {
    // Approximation of cumulative normal distribution
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

// European Option
EuropeanOption::EuropeanOption(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
                               double notional, const std::string& instrument, double strike,
                               const std::string& optionType)
    : Option(id, "european", tradeDate, startDate, endDate, notional, instrument, strike, optionType) {}

double EuropeanOption::calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                                  const std::shared_ptr<VolatilityCurve>& volCurve,
                                  const std::map<std::string, double>& stockPrices) const {
    
    auto stockIt = stockPrices.find(instrument_);
    if (stockIt == stockPrices.end()) {
        throw std::runtime_error("Stock price not found: " + instrument_);
    }
    
    double spot = stockIt->second;
    double timeToMaturity = Date(2025, 1, 1).yearFraction(endDate_);
    double riskFreeRate = irCurve->getZeroRate(endDate_);
    double volatility = volCurve->getVolatility(endDate_);
    bool isCall = (optionType_ == "call");
    
    // Use Black-Scholes for comparison, but also calculate binomial tree
    double bsPrice = blackScholesPrice(spot, strike_, timeToMaturity, riskFreeRate, volatility, isCall);
    double binomialPrice = binomialTreePrice(spot, strike_, timeToMaturity, riskFreeRate, volatility, isCall, 50);
    
    return binomialPrice * notional_ / 100.0; // Scale by notional
}

double EuropeanOption::binomialTreePrice(double spot, double strike, double timeToMaturity,
                                        double riskFreeRate, double volatility, bool isCall, int steps) const {
    
    double dt = timeToMaturity / steps;
    double u = std::exp(volatility * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp(riskFreeRate * dt) - d) / (u - d);
    
    // Initialize asset prices at maturity
    std::vector<double> assetPrices(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        assetPrices[i] = spot * std::pow(u, steps - i) * std::pow(d, i);
    }
    
    // Calculate option values at maturity
    std::vector<double> optionValues(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        if (isCall) {
            optionValues[i] = std::max(0.0, assetPrices[i] - strike);
        } else {
            optionValues[i] = std::max(0.0, strike - assetPrices[i]);
        }
    }
    
    // Backward induction
    for (int step = steps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            optionValues[i] = std::exp(-riskFreeRate * dt) * (p * optionValues[i] + (1 - p) * optionValues[i + 1]);
        }
    }
    
    return optionValues[0];
}

// American Option
AmericanOption::AmericanOption(int id, const Date& tradeDate, const Date& startDate, const Date& endDate,
                               double notional, const std::string& instrument, double strike,
                               const std::string& optionType)
    : Option(id, "american", tradeDate, startDate, endDate, notional, instrument, strike, optionType) {}

double AmericanOption::calculatePV(const std::shared_ptr<InterestRateCurve>& irCurve,
                                  const std::shared_ptr<VolatilityCurve>& volCurve,
                                  const std::map<std::string, double>& stockPrices) const {
    
    auto stockIt = stockPrices.find(instrument_);
    if (stockIt == stockPrices.end()) {
        throw std::runtime_error("Stock price not found: " + instrument_);
    }
    
    double spot = stockIt->second;
    double timeToMaturity = Date(2025, 1, 1).yearFraction(endDate_);
    double riskFreeRate = irCurve->getZeroRate(endDate_);
    double volatility = volCurve->getVolatility(endDate_);
    bool isCall = (optionType_ == "call");
    
    double price = americanBinomialTreePrice(spot, strike_, timeToMaturity, riskFreeRate, volatility, isCall, 50);
    return price * notional_ / 100.0;
}

double AmericanOption::americanBinomialTreePrice(double spot, double strike, double timeToMaturity,
                                               double riskFreeRate, double volatility, bool isCall, int steps) const {
    
    double dt = timeToMaturity / steps;
    double u = std::exp(volatility * std::sqrt(dt));
    double d = 1.0 / u;
    double p = (std::exp(riskFreeRate * dt) - d) / (u - d);
    
    // Initialize asset prices at maturity
    std::vector<double> assetPrices(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        assetPrices[i] = spot * std::pow(u, steps - i) * std::pow(d, i);
    }
    
    // Calculate option values at maturity
    std::vector<double> optionValues(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        if (isCall) {
            optionValues[i] = std::max(0.0, assetPrices[i] - strike);
        } else {
            optionValues[i] = std::max(0.0, strike - assetPrices[i]);
        }
    }
    
    // Backward induction with early exercise
    for (int step = steps - 1; step >= 0; --step) {
        for (int i = 0; i <= step; ++i) {
            // Calculate asset price at this node
            double currentAssetPrice = spot * std::pow(u, step - i) * std::pow(d, i);
            
            // Calculate continuation value
            double continuationValue = std::exp(-riskFreeRate * dt) * (p * optionValues[i] + (1 - p) * optionValues[i + 1]);
            
            // Calculate immediate exercise value
            double exerciseValue;
            if (isCall) {
                exerciseValue = std::max(0.0, currentAssetPrice - strike);
            } else {
                exerciseValue = std::max(0.0, strike - currentAssetPrice);
            }
            
            // American option: take maximum of continuation and exercise
            optionValues[i] = std::max(continuationValue, exerciseValue);
        }
    }
    
    return optionValues[0];
}