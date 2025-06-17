#ifndef RISK_ENGINE_H
#define RISK_ENGINE_H

#include "Trade.h"
#include "Curve.h"
#include <vector>
#include <memory>
#include <map>

struct Greeks {
    double delta;
    double vega;
    
    Greeks() : delta(0.0), vega(0.0) {}
    Greeks(double d, double v) : delta(d), vega(v) {}
};

class RiskEngine {
private:
    std::vector<std::unique_ptr<Trade>> trades_;
    std::shared_ptr<InterestRateCurve> usdCurve_;
    std::shared_ptr<InterestRateCurve> sgdCurve_;
    std::shared_ptr<VolatilityCurve> volCurve_;
    std::map<std::string, double> stockPrices_;
    
public:
    RiskEngine(std::shared_ptr<InterestRateCurve> usdCurve,
               std::shared_ptr<InterestRateCurve> sgdCurve,
               std::shared_ptr<VolatilityCurve> volCurve);
    
    void addTrade(std::unique_ptr<Trade> trade);
    void loadStockPrices(const std::string& filename);
    
    double calculatePortfolioPV() const;
    Greeks calculateTradeGreeks(const Trade* trade) const;
    
    void generateReport(const std::string& filename) const;
    
private:
    std::shared_ptr<InterestRateCurve> bumpCurve(const std::shared_ptr<InterestRateCurve>& curve, double bumpSize) const;
    std::shared_ptr<VolatilityCurve> bumpVolCurve(const std::shared_ptr<VolatilityCurve>& curve, double bumpSize) const;
    
    std::shared_ptr<InterestRateCurve> getRelevantCurve(const std::string& instrument) const;
};

#endif