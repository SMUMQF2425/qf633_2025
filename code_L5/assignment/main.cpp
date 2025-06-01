#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <ctime>
#include <chrono>
#include <memory>

#include "Market.h"
#include "Pricer.h"
#include "EuropeanTrade.h"
#include "Bond.h"
#include "Swap.h"
#include "AmericanTrade.h"
#include "TradeFactory.h"

using namespace std;

void readFromFile(const string &fileName, string &outPut)
{
	string lineText;
	ifstream MyReadFile(fileName);
	while (getline(MyReadFile, lineText))
	{
		outPut.append(lineText);
	}
	MyReadFile.close();
}

int main()
{
	// task 1, create an market data object, and update the market data from from txt file
	tm now_;
	Date valueDate;
	time_t t = time(nullptr);
	struct tm timeInfo;
	if (localtime_r(&t, &now_) == 0)
	{
		// localtime_r() convert current system time into localtime and populate tm struct
		valueDate.year = timeInfo.tm_year + 1900; // 1900 based
		valueDate.month = timeInfo.tm_mon + 1;	  // 0 based
		valueDate.day = timeInfo.tm_mday;
	};
	cout << valueDate << endl;
	// Date newDate;
	// cin >> newDate;
	// cout << newDate;

	Market mkt0;
	Market mkt1 = Market(valueDate);
	Market mkt2(mkt1);
	Market mkt3;
	mkt3 = mkt2; // assignemnt constructor

	/*
	load data from file and update market object with data
	*/

	// --- Load bond prices ---
	{
		std::ifstream fin("bondPrice.txt");
		std::string line;
		while (getline(fin, line))
		{
			if (line.empty())
				continue;
			size_t pos = line.find(':');
			if (pos == std::string::npos)
				continue;
			std::string name = line.substr(0, pos);
			double price = atof(line.substr(pos + 1).c_str());
			mkt1.addBondPrice(name, price);
		}
	}

	// --- Load stock prices ---
	{
		std::ifstream fin("stockPrice.txt");
		std::string line;
		while (getline(fin, line))
		{
			if (line.empty())
				continue;
			size_t pos = line.find(':');
			if (pos == std::string::npos)
				continue;
			std::string name = line.substr(0, pos);
			double price = atof(line.substr(pos + 1).c_str());
			mkt1.addVolCurve(name, price); // This actually adds to stockPrices
		}
	}

	// --- Load rate curve ---
	{
		std::ifstream fin("curve.txt");
		std::string line;
		std::string curveName;
		RateCurve curve;
		bool first = true;
		while (getline(fin, line))
		{
			if (line.empty())
				continue;
			if (first)
			{
				curveName = line;
				curve = RateCurve(curveName);
				first = false;
			}
			else
			{
				size_t pos = line.find(':');
				if (pos == std::string::npos)
					continue;
				std::string tenorStr = line.substr(0, pos);
				std::string rateStr = line.substr(pos + 1);
				double rate = atof(rateStr.c_str()) / 100.0; // convert % to decimal
				// crude date parsing: e.g., "1Y" -> Date(valueDate.year+1, valueDate.month, valueDate.day)
				int years = 0, months = 0;
				if (tenorStr.find('Y') != std::string::npos)
					years = atoi(tenorStr.c_str());
				else if (tenorStr.find('M') != std::string::npos)
					months = atoi(tenorStr.c_str());
				Date tenorDate = valueDate;
				tenorDate.year += years;
				tenorDate.month += months;
				curve.addRate(tenorDate, rate);
			}
		}
		mkt1.addCurve(curveName, curve);
	}

	// --- Load vol curve ---
	{
		std::ifstream fin("vol.txt");
		std::string line;
		VolCurve vol("APPL"); // hardcoded for APPL, generalize as needed
		while (getline(fin, line))
		{
			if (line.empty())
				continue;
			size_t pos = line.find(':');
			if (pos == std::string::npos)
				continue;
			std::string tenorStr = line.substr(0, pos);
			std::string volStr = line.substr(pos + 1);
			double v = atof(volStr.c_str()) / 100.0;
			int years = 0, months = 0;
			if (tenorStr.find('Y') != std::string::npos)
				years = atoi(tenorStr.c_str());
			else if (tenorStr.find('M') != std::string::npos)
				months = atoi(tenorStr.c_str());
			Date tenorDate = valueDate;
			tenorDate.year += years;
			tenorDate.month += months;
			vol.addVol(tenorDate, v);
		}
		mkt1.addVolCurve("APPL", vol);
	}

	// task 2, create a portfolio of bond, swap, european option, american option
	// for each time, at least should have long / short, different tenor or expiry, different underlying
	// totally no less than 16 trades

	// Use smart pointers and factories
	vector<shared_ptr<Trade>> myPortfolio;
	auto lFactory = make_unique<LinearTradeFactory>();
	auto oFactory = make_unique<OptionTradeFactory>();

	// Load trades from trade.txt using factories and setters
	std::ifstream tradeFile("trade.txt");
	std::string line;
	getline(tradeFile, line); // skip header
	while (getline(tradeFile, line))
	{
		std::stringstream ss(line);
		std::string token;
		std::vector<std::string> fields;
		while (getline(ss, token, ';'))
			fields.push_back(token);
		if (fields.size() < 11)
			continue;
		std::string type = fields[1];
		Date trade_dt, start_dt, end_dt;
		sscanf(fields[2].c_str(), "%d-%d-%d", &trade_dt.year, &trade_dt.month, &trade_dt.day);
		sscanf(fields[3].c_str(), "%d-%d-%d", &start_dt.year, &start_dt.month, &start_dt.day);
		sscanf(fields[4].c_str(), "%d-%d-%d", &end_dt.year, &end_dt.month, &end_dt.day);
		double notional = atof(fields[5].c_str());
		std::string instrument = fields[6];
		double rate = atof(fields[7].c_str());
		double strike = atof(fields[8].c_str());
		double freq = atof(fields[9].c_str());
		std::string option = fields[10];

		if (type == "bond")
		{
			auto bond = lFactory->createTrade("bond", trade_dt, end_dt);
			auto bondPtr = dynamic_pointer_cast<Bond>(bond);
			if (bondPtr)
			{
				bondPtr->setBondName(instrument);
				bondPtr->setNotional(notional);
				bondPtr->setTradePrice(rate);
				bondPtr->setFrequency(int(1.0 / freq));
				bondPtr->setStartDate(start_dt);
				bondPtr->setEndDate(end_dt);
				myPortfolio.push_back(bond);
			}
		}
		else if (type == "swap")
		{
			auto swap = lFactory->createTrade("swap", trade_dt, end_dt);
			auto swapPtr = dynamic_pointer_cast<Swap>(swap);
			if (swapPtr)
			{
				swapPtr->setNotional(notional);
				swapPtr->setRate(rate);
				swapPtr->setFrequency(int(1.0 / freq));
				swapPtr->setStartDate(start_dt);
				swapPtr->setEndDate(end_dt);
				myPortfolio.push_back(swap);
			}
		}
		else if (type == "european")
		{
			auto eoption = oFactory->createTrade("european", trade_dt, end_dt);
			auto eOpt = dynamic_pointer_cast<EuropeanOption>(eoption);
			if (eOpt)
			{
				eOpt->setStrike(strike);
				eOpt->setOptionType((option == "call") ? Call : Put);
				eOpt->setUnderlying(instrument);
				eOpt->setExpiry(end_dt);
				myPortfolio.push_back(eoption);
			}
		}
		else if (type == "american")
		{
			auto aoption = oFactory->createTrade("american", trade_dt, end_dt);
			auto aOpt = dynamic_pointer_cast<AmericanOption>(aoption);
			if (aOpt)
			{
				aOpt->setStrike(strike);
				aOpt->setOptionType((option == "call") ? Call : Put);
				aOpt->setUnderlying(instrument);
				aOpt->setExpiry(end_dt);
				myPortfolio.push_back(aoption);
			}
		}
	}

	// task 3, create a pricer and price the portfolio, output the pricing result of each deal.
	auto treePricer = make_unique<CRRBinomialTreePricer>(50);
	std::ofstream out("result.txt");
	for (auto &trade : myPortfolio)
	{
		double pv = treePricer->Price(mkt1, trade.get());
		out << trade->getType() << ": " << pv << std::endl;
	}
	out.close();

	// task 4, analyzing pricing result
	//  a) compare CRR binomial tree result for an european option vs Black model
	//  b) compare CRR binomial tree result for an american option call vs european option call, and put

	// final
	cout << "Project build successfully!" << endl;
	return 0;
}
