#include "RiskEngine.h"
#include "TradeFactory.h"
#include "Curve.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

std::vector<std::string> splitLine(const std::string& line, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

int main() {
    try {
        // Initialize curves
        auto usdCurve = std::make_shared<InterestRateCurve>("USD-SOFR");
        auto sgdCurve = std::make_shared<InterestRateCurve>("SGD-SORA");
        auto volCurve = std::make_shared<VolatilityCurve>();
        
        // Load market data
        usdCurve->loadFromFile("usd_curve.txt");
        sgdCurve->loadFromFile("sgd_curve.txt");
        volCurve->loadFromFile("vol.txt");
        
        // Initialize risk engine
        RiskEngine riskEngine(usdCurve, sgdCurve, volCurve);
        riskEngine.loadStockPrices("stockPrice.txt");
        
        // Load trades
        std::ifstream tradeFile("trade.txt");
        if (!tradeFile.is_open()) {
            throw std::runtime_error("Cannot open trade file");
        }
        
        std::string line;
        std::getline(tradeFile, line); // Skip header
        
        while (std::getline(tradeFile, line)) {
            auto tradeData = splitLine(line, ';');
            if (tradeData.size() >= 11) {
                auto trade = TradeFactory::createTrade(tradeData);
                riskEngine.addTrade(std::move(trade));
            }
        }
        
        // Generate results
        riskEngine.generateReport("results.txt");
        
        std::cout << "Derivatives pricing completed successfully!\n";
        std::cout << "Results written to results.txt\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}