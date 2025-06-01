#pragma once
#include "Trade.h"

class Swap : public Trade
{
public:
	// make necessary change
	Swap(const Date &tradeDate, Date start, Date end, double notional, double rate, int freq) : Trade("SwapTrade", tradeDate)
	{
		startDate = start;
		endDate = end;
		swapNotional = notional;
		tradeRate = rate;
		frequency = freq;
	}

	inline double Payoff(double marketRate) const
	{
		double annuity = getAnnuity();
		return annuity * (tradeRate - marketRate);
	}
	inline void setNotional(double notional) { swapNotional = notional; }
	inline void setRate(double rate) { tradeRate = rate; }
	inline void setFrequency(int freq) { frequency = freq; }
	inline void setStartDate(const Date& d) { startDate = d; }
	inline void setEndDate(const Date& d) { endDate = d; }

private:
	Date startDate;
	Date endDate;
	double swapNotional;
	double tradeRate;
	int frequency; // use 1 for annual, 2 for semi-annual etc

	double getAnnuity() const
	{
		// Assume zero rate is 4% per annum for discounting
		double z = 0.04;
		double annuity = 0.0;
		double dt = 1.0 / frequency;
		int n = static_cast<int>((endDate - startDate) * frequency);
		for (int i = 1; i <= n; ++i)
		{
			double t = i * dt;
			double df = exp(-z * t);
			annuity += swapNotional * dt * df;
		}
		return annuity;
	}
};