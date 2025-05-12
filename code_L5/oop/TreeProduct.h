#ifndef _TREE_PRODUCT_H
#define _TREE_PRODUCT_H
#include "Date.h"
#include "Trade.h"

// this class provide a common member function interface for option type of trade.
class TreeProduct: public Trade
{
public:
    TreeProduct(): Trade() { tradeType = "TreeProduct";};
    virtual const Date& GetExpiry() const = 0;
    virtual double ValueAtNode(double stockPrice, double t, double continuationValue) const = 0;
};

#endif
