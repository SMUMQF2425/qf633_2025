#pragma once
#include <memory>
#include <string>
#include "Trade.h"
#include "Swap.h"
#include "Bond.h"
#include "EuropeanTrade.h"
#include "AmericanTrade.h"

using namespace std;

// Abstract creator class
class TradeFactory
{
public:
    virtual shared_ptr<Trade> createTrade(const string &type, const Date &tradeDate, const Date &expiryDate) = 0;
    virtual ~TradeFactory()
    {
        cout << "trade factory is destroyed" << endl;
    }
};

// Concrete creator class - create linear trades
class LinearTradeFactory : public TradeFactory
{
public:
    shared_ptr<Trade> createTrade(const string &type, const Date &tradeDate, const Date &expiryDate) override
    {
        if (type == "swap")
        {
            return make_shared<Swap>(tradeDate, tradeDate, expiryDate, 0, 0, 1); // Setters will be used later
        }
        else if (type == "bond")
        {
            return make_shared<Bond>("", tradeDate, tradeDate, expiryDate, 0, 1, 0); // Setters will be used later
        }
        else
        {
            return nullptr;
        }
    }
};

// Concrete creator class - create option trades
class OptionTradeFactory : public TradeFactory
{
public:
    shared_ptr<Trade> createTrade(const string &type, const Date &tradeDate, const Date &expiryDate) override
    {
        if (type == "european")
        {
            return make_shared<EuropeanOption>(Call, 0, expiryDate, ""); // Setters will be used later
        }
        else if (type == "american")
        {
            return make_shared<AmericanOption>(Call, 0, expiryDate, ""); // Setters will be used later
        }
        else
        {
            return nullptr;
        }
    }
};