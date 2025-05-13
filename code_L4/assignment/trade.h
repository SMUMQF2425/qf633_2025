#pragma once

#include <string>
#include <iostream>
#include "date.h"

class OptionTrade {
public:
    OptionTrade(double notional, double strike, bool isCall,
                const std::string& start, const std::string& end);
    double calculatePv(double spot, double vol, double rate);
    std::string getTradeDetails() const;

private:
    double m_notional;
    double m_strike;
    bool m_isCall;
    std::string m_startDate;
    std::string m_endDate;
};