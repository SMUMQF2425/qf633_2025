#include "RiskEngine.h"
#include "TradeFactory.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

RiskEngine::RiskEngine(std::shared_ptr<InterestRateCurve> usdCurve,
                       std::shared_ptr<InterestRateCurve> sgdCurve,
                       std::shared_ptr<VolatilityCurve> volCurve)
    : usdCurve_(usdCurve), sgdCurve_(sgdCurve), volCurve_(volCurve) {}

void RiskEngine::addTrade(std::unique_ptr<Trade> trade) {
    trades_.push_back(std::move(trade));
}

void RiskEngine::loadStockPrices(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open stock price file: " + filename);
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string symbol, priceStr;
        
        std::getline(iss, symbol, ':');
        std::getline(iss, priceStr);
        
        // Clean up strings
        symbol.erase(std::remove_if(symbol.begin(), symbol.end(), ::isspace), symbol.end());
        priceStr.erase(std::remove_if(priceStr.begin(), priceStr.end(), ::isspace), priceStr.end());
        
        stockPrices_[symbol] = std::stod(priceStr);
    }
}

double RiskEngine::calculatePortfolioPV() const {
    double totalPV = 0.0;
    
    for (const auto& trade : trades_) {
        auto relevantCurve = getRelevantCurve(trade->getInstrument());
        double tradePV = trade->calculatePV(relevantCurve, volCurve_, stockPrices_);
        totalPV += tradePV;
    }
    
    return totalPV;
}

Greeks RiskEngine::calculateTradeGreeks(const Trade* trade) const {
    auto relevantCurve = getRelevantCurve(trade->getInstrument());
    double basePV = trade->calculatePV(relevantCurve, volCurve_, stockPrices_);

    double delta = 0.0;
    if (trade->getType() == "european" || trade->getType() == "american") {
        // Bump the stock price for options
        auto bumpedStockPrices = stockPrices_;
        auto it = bumpedStockPrices.find(trade->getInstrument());
        if (it != bumpedStockPrices.end()) {
            double bumpSize = 1.0; // $1 bump
            it->second += bumpSize;
            double bumpedPV = trade->calculatePV(relevantCurve, volCurve_, bumpedStockPrices);
            delta = (bumpedPV - basePV) / bumpSize;
        }
    } else {
        // Bump the interest rate curve for swaps/bonds
        double bumpSize = 0.0001;
        auto bumpedCurve = bumpCurve(relevantCurve, bumpSize);
        double bumpedPV = trade->calculatePV(bumpedCurve, volCurve_, stockPrices_);
        delta = bumpedPV - basePV;
    }

    // Vega calculation (unchanged)
    double volBumpSize = 0.01;
    auto bumpedVolCurve = bumpVolCurve(volCurve_, volBumpSize);
    double volBumpedPV = trade->calculatePV(relevantCurve, bumpedVolCurve, stockPrices_);
    double vega = volBumpedPV - basePV;

    return Greeks(delta, vega);
}

void RiskEngine::generateReport(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create output file: " + filename);
    }
    
    file << "Trade_ID,Type,PV,Delta,Vega\n";
    
    for (const auto& trade : trades_) {
        auto relevantCurve = getRelevantCurve(trade->getInstrument());
        double pv = trade->calculatePV(relevantCurve, volCurve_, stockPrices_);
        Greeks greeks = calculateTradeGreeks(trade.get());
        
        file << trade->getId() << ","
             << trade->getType() << ","
             << std::fixed << std::setprecision(2) << pv << ","
             << std::fixed << std::setprecision(2) << greeks.delta << ","
             << std::fixed << std::setprecision(2) << greeks.vega << "\n";
    }
    
    // Portfolio totals
    double totalPV = calculatePortfolioPV();
    double totalDelta = 0.0, totalVega = 0.0;
    
    for (const auto& trade : trades_) {
        Greeks greeks = calculateTradeGreeks(trade.get());
        totalDelta += greeks.delta;
        totalVega += greeks.vega;
    }
    
    file << "\nPortfolio,Total,"
         << std::fixed << std::setprecision(2) << totalPV << ","
         << std::fixed << std::setprecision(2) << totalDelta << ","
         << std::fixed << std::setprecision(2) << totalVega << "\n";
    
    file.close();
}

std::shared_ptr<InterestRateCurve> RiskEngine::bumpCurve(
    const std::shared_ptr<InterestRateCurve>& curve, double bumpSize) const {
    
    // Create a new curve with the same name
    auto bumpedCurve = std::make_shared<InterestRateCurve>(curve->getCurveName());
    auto curvePoints = curve->getAllCurvePoints();
    
    // Bump each rate and add to new curve
    for (const auto& point : curvePoints) {
        Date maturityDate = point.first;
        double originalRate = point.second;
        double bumpedRate = originalRate + bumpSize;
        
        bumpedCurve->addPoint(maturityDate, bumpedRate);
    }
    
    return bumpedCurve;
}

std::shared_ptr<VolatilityCurve> RiskEngine::bumpVolCurve(
    const std::shared_ptr<VolatilityCurve>& curve, double bumpSize) const {
    
    // Create a new volatility curve
    auto bumpedCurve = std::make_shared<VolatilityCurve>();
    auto volPoints = curve->getAllVolPoints();
    
    // Bump each volatility and add to new curve
    for (const auto& point : volPoints) {
        Date maturityDate = point.first;
        double originalVol = point.second;
        double bumpedVol = originalVol + bumpSize;
        
        // Ensure volatility doesn't go negative
        bumpedVol = std::max(0.001, bumpedVol); // Minimum 0.1% volatility
        
        bumpedCurve->addPoint(maturityDate, bumpedVol);
    }
    
    return bumpedCurve;
}

std::shared_ptr<InterestRateCurve> RiskEngine::getRelevantCurve(const std::string& instrument) const {
    if (instrument.find("usd") != std::string::npos || instrument.find("USD") != std::string::npos) {
        return usdCurve_;
    } else if (instrument.find("sgd") != std::string::npos || instrument.find("SGD") != std::string::npos) {
        return sgdCurve_;
    } else {
        // For stocks, use USD curve as default
        return usdCurve_;
    }
}