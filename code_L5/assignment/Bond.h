#pragma once
#include "Trade.h"
#include "Market.h"

class Bond : public Trade
{

public:
	Bond(const std::string &name, const Date &tradeDate, const Date &start, const Date &end,
		 double notional, int couponFreq, double price) : Trade("BondTrade", tradeDate)
	{
		startDate = start;
		endDate = end;
		bondName = name;
		bondNotional = notional;
		frequecy = couponFreq;
		tradePrice = price;
	}

	inline double Payoff(double marketPrice) const
	{
		return (marketPrice - tradePrice) / 100.0 * bondNotional;
	}

	inline std::string getBondName() const { return bondName; } // implement this
	inline void setBondName(const std::string &name) { bondName = name; }
	inline void setNotional(double notional) { bondNotional = notional; }
	inline void setTradePrice(double price) { tradePrice = price; }
	inline void setFrequency(int freq) { frequecy = freq; }
	inline void setStartDate(const Date &d) { startDate = d; }
	inline void setEndDate(const Date &d) { endDate = d; }
	inline void setCoupon(double c) { coupon = c; }

private:
	std::string bondName;
	double bondNotional;
	double tradePrice;
	int frequecy;
	Date startDate;
	Date endDate;
	double coupon = 0.0; // <-- Add this line
};
