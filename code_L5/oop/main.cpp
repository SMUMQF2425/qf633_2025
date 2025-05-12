#include <fstream>
#include <ctime>
#include <chrono>

#include "Market.h"
#include "Pricer.h"
#include "EuropeanTrade.h"
#include "Bond.h"
#include "Swap.h"
#include "AmericanTrade.h"

using namespace std;

void readFromFile(const string& fileName, string& outPut) {
	string lineText;
	ifstream MyReadFile(fileName);
	while (getline(MyReadFile, lineText)) {
		outPut.append(lineText);
	}
	MyReadFile.close();
}

int main()
{
	//task 1, create an market data object, and update the market data from from txt file
	Date valueDate;
	time_t t = time(nullptr);
	struct tm timeInfo;
	if (localtime_s(&timeInfo, &t) == 0) {
		// localtime_s() convert current system time into localtime and populate tm struct
		valueDate.year = timeInfo.tm_year + 1900; //1900 based
		valueDate.month = timeInfo.tm_mon + 1; //0 based
		valueDate.day = timeInfo.tm_mday;
	};
	cout << valueDate << endl;
	//Date newDate;
	//cin >> newDate;
	//cout << newDate;
	
	Market mkt0;
	Market mkt1 = Market(valueDate);
	Market mkt2(mkt1);
	Market mkt3;
	mkt3 = mkt2; //assignemnt constructor	

	/*
	load data from file and update market object with data
	*/

	//task 2, create a portfolio of bond, swap, european option, american option
	//for each time, at least should have long / short, different tenor or expiry, different underlying
	//totally no less than 16 trades
	vector<Trade*> myPortfolio;
	Trade* bond = new Bond("tesst", Date(2023, 12, 31), Date(2024, 1, 1), Date(2034, 1, 1), 10000000, 2, 103.5);
	myPortfolio.push_back(bond);


	//task 3, creat a pricer and price the portfolio, output the pricing result of each deal.
	Pricer* treePricer = new CRRBinomialTreePricer(10);
	for (auto trade : myPortfolio) {
		double pv = treePricer->Price(mkt1, trade);
		//log pv details out in a file

	}

	//task 4, analyzing pricing result
	// a) compare CRR binomial tree result for an european option vs Black model
	// b) compare CRR binomial tree result for an american option call vs european option call, and put

	//final
	cout << "Project build successfully!" << endl;
	return 0;

}
