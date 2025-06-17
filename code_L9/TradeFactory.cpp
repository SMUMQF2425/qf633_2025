#include "TradeFactory.h"
#include <sstream>
#include <stdexcept>

std::unique_ptr<Trade> TradeFactory::createTrade(const std::vector<std::string>& tradeData) {
    if (tradeData.size() < 11) {
        throw std::invalid_argument("Insufficient trade data");
    }
    
    int id = std::stoi(tradeData[0]);
    std::string type = tradeData[1];
    Date tradeDate = parseDate(tradeData[2]);
    Date startDate = parseDate(tradeData[3]);
    Date endDate = parseDate(tradeData[4]);
    double notional = parseDouble(tradeData[5]);
    std::string instrument = tradeData[6];
    double rate = parseDouble(tradeData[7]);
    double strike = parseDouble(tradeData[8]);
    double frequency = parseDouble(tradeData[9]);
    std::string optionType = tradeData[10];
    
    if (type == "swap") {
        return std::make_unique<Swap>(id, tradeDate, startDate, endDate, notional, instrument, rate, frequency);
    } else if (type == "bond") {
        return std::make_unique<Bond>(id, tradeDate, startDate, endDate, notional, instrument, rate, frequency);
    } else if (type == "european") {
        return std::make_unique<EuropeanOption>(id, tradeDate, startDate, endDate, notional, instrument, strike, optionType);
    } else if (type == "american") {
        return std::make_unique<AmericanOption>(id, tradeDate, startDate, endDate, notional, instrument, strike, optionType);
    } else {
        throw std::invalid_argument("Unknown trade type: " + type);
    }
}

Date TradeFactory::parseDate(const std::string& dateStr) {
    return Date(dateStr);
}

double TradeFactory::parseDouble(const std::string& str) {
    if (str.empty() || str == "0" || str == "na") return 0.0;
    return std::stod(str);
}