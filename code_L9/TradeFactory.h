#ifndef TRADE_FACTORY_H
#define TRADE_FACTORY_H

#include "Trade.h"
#include <memory>
#include <string>
#include <vector>

// Factory pattern implementation
class TradeFactory {
public:
    static std::unique_ptr<Trade> createTrade(const std::vector<std::string>& tradeData);
    
private:
    static Date parseDate(const std::string& dateStr);
    static double parseDouble(const std::string& str);
};

#endif