#include <cmath>
#include "Pricer.h"
#include "Bond.h" // Add this include!
#include "Swap.h" // Add this include if you want to handle swaps
#include "TreeProduct.h"

double Pricer::Price(const Market &mkt, Trade *trade)
{
	double pv = 0;
	if (trade->getType() == "TreeProduct")
	{
		TreeProduct *treePtr = dynamic_cast<TreeProduct *>(trade);
		if (treePtr)
		{
			pv = PriceTree(mkt, *treePtr);
		}
	}
	else if (trade->getType() == "BondTrade")
	{
		const Bond *bond = dynamic_cast<const Bond *>(trade);
		if (bond)
		{
			// Try to fetch bond price from market using bondName
			// You may need to add a getter for bondName in Bond.h if not present
			std::string bondName = bond->getBondName(); // Add getBondName() in Bond.h if missing
			double marketPrice = 100;
			try
			{
				marketPrice = mkt.getBondPrice(bondName); // Add getBondPrice() in Market.h/cpp if missing
			}
			catch (...)
			{
			}
			pv = bond->Payoff(marketPrice);
		}
	}
	else if (trade->getType() == "SwapTrade")
	{
		const Swap *swap = dynamic_cast<const Swap *>(trade);
		if (swap)
		{
			// For demo, use a constant market rate or fetch from market
			double marketRate = 0.04; // Or fetch from mkt
			pv = swap->Payoff(marketRate);
		}
	}
	else
	{
		double price = 0;
		pv = trade->Payoff(price);
	}
	return pv;
}

void BinomialTreePricer::ModelSetup(double S0, double sigma, double r, double dt)
{
	// a basic version of binomial tree
	u = 1.1;
	d = 0.9;
	p = (exp(r) - d) / (u - d);
}

double BinomialTreePricer::PriceTree(const Market &mkt, const TreeProduct &trade)
{
	double T = trade.GetExpiry() - mkt.asOf;
	double dt = T / nTimeSteps;

	// Get the instrument name (e.g., APPL) from the trade if possible
	std::string instrument = trade.getUnderlying();

	double stockPrice = mkt.getStockPrice(instrument);
	double vol = mkt.getVolCurve(instrument).getVol(trade.GetExpiry());
	double rate = 0.04; // or use mkt.getCurve(...).getRate(trade.GetExpiry());

	ModelSetup(stockPrice, vol, rate, dt);

	if (stockPrice <= 0 || vol <= 0)
	{
		std::cerr << "Warning: Invalid market data for " << instrument
				  << " (spot=" << stockPrice << ", vol=" << vol << ")" << std::endl;
	}

	for (int i = 0; i <= nTimeSteps; i++)
	{
		states[i] = trade.Payoff(GetSpot(nTimeSteps, i));
	}

	for (int k = nTimeSteps - 1; k >= 0; k--)
		for (int i = 0; i <= k; i++)
		{
			double df = exp(-rate * dt);
			double continuation = df * (states[i] * GetProbUp() + states[i + 1] * GetProbDown());
			states[i] = trade.ValueAtNode(GetSpot(k, i), dt * k, continuation);
		}

	return states[0];
}

void CRRBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
	u = exp(sigma * sqrt(dt));
	d = exp(-sigma * sqrt(dt));
	p = (exp(rate * dt) - d) / (u - d);
	currentSpot = S0;
}

void JRRNBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
	u = std::exp((rate - sigma * sigma / 2) * dt + sigma * std::sqrt(dt));
	d = std::exp((rate - sigma * sigma / 2) * dt - sigma * std::sqrt(dt));
	p = (std::exp(rate * dt) - d) / (u - d);
	currentSpot = S0;
}
