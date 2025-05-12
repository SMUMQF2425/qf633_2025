#include "trade.h"
#include "black.h"
#include <sstream>
#include <iomanip>

OptionTrade::OptionTrade(double notional, double strike, bool isCall,
                        const std::string& start, const std::string& end)
    : m_notional(notional), m_strike(strike), m_isCall(isCall),
      m_startDate(start), m_endDate(end) {}

double OptionTrade::calculatePv(double spot, double vol, double rate) {
    auto parseDate = [](const std::string& s) -> int {
        int year = stoi(s.substr(0, 4));
        int month = stoi(s.substr(5, 2));
        int day = stoi(s.substr(8, 2));
        return year * 365 + month * 30 + day;
    };

    int days = parseDate(m_endDate) - parseDate(m_startDate);
    double expiry = days / 365.0;
    return BlackScholes(m_notional, m_strike, expiry, spot, vol, rate, m_isCall);
}

std::string OptionTrade::getTradeDetails() const {
    std::stringstream ss;
    ss << "Notional: " << m_notional << ", Strike: " << m_strike 
       << ", " << (m_isCall ? "Call" : "Put");
    return ss.str();
}