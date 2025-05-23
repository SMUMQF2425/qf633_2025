#include "Market.h"

using namespace std;

void RateCurve::display() const
{
  cout << "rate curve:" << name << endl;
  for (size_t i = 0; i < tenorDates.size(); i++)
  {
    cout << tenorDates[i] << ":" << rates[i] << endl;
  }
  cout << endl;
}

void RateCurve::addRate(Date tenor, double rate)
{
  // consider to check if tenor already exist
  if (true)
  {
    tenorDates.push_back(tenor);
    rates.push_back(rate);
  }
}

double RateCurve::getRate(Date tenor) const
{
  if (tenorDates.empty())
    return 0;
  if (tenor - tenorDates.front() <= 0)
    return rates.front();
  if (tenor - tenorDates.back() >= 0)
    return rates.back();
  for (size_t i = 1; i < tenorDates.size(); ++i)
  {
    if ((tenor - tenorDates[i - 1] >= 0) && (tenor - tenorDates[i] <= 0))
    {
      double t1 = tenorDates[i - 1] - tenorDates[0];
      double t2 = tenorDates[i] - tenorDates[0];
      double t = tenor - tenorDates[0];
      double r1 = rates[i - 1], r2 = rates[i];
      return r1 + (r2 - r1) * (t - t1) / (t2 - t1);
    }
  }
  return rates.back();
}

void VolCurve::addVol(Date tenor, double vol)
{
  tenors.push_back(tenor);
  vols.push_back(vol);
}

double VolCurve::getVol(Date tenor) const
{
  if (tenors.empty())
    return 0;
  if (tenor - tenors.front() <= 0)
    return vols.front();
  if (tenor - tenors.back() >= 0)
    return vols.back();
  for (size_t i = 1; i < tenors.size(); ++i)
  {
    if ((tenor - tenors[i - 1] >= 0) && (tenor - tenors[i] <= 0))
    {
      double t1 = tenors[i - 1] - tenors[0];
      double t2 = tenors[i] - tenors[0];
      double t = tenor - tenors[0];
      double v1 = vols[i - 1], v2 = vols[i];
      return v1 + (v2 - v1) * (t - t1) / (t2 - t1);
    }
  }
  return vols.back();
}

void VolCurve::display() const
{
  std::cout << "Vol curve: " << name << std::endl;
  for (size_t i = 0; i < tenors.size(); ++i)
  {
    std::cout << tenors[i] << ": " << vols[i] << std::endl;
  }
  std::cout << std::endl;
}

void Market::Print() const
{
  cout << "market asof: " << asOf << endl;

  for (auto curve : curves)
  {
    curve.second.display();
  }
  for (auto vol : vols)
  {
    vol.second.display();
  }
  /*
  add display for bond price and stock price

  */
}

void Market::addCurve(const std::string &curveName, const RateCurve &curve)
{
  curves.emplace(curveName, curve);
}

void Market::addVolCurve(const std::string &curveName, const VolCurve &curve)
{
  vols.emplace(curveName, curve);
}

void Market::addBondPrice(const std::string &bondName, double price)
{
  bondPrices[bondName] = price;
}

void Market::addVolCurve(const std::string &stockName, double price)
{
  stockPrices[stockName] = price;
}

double Market::getBondPrice(const std::string &bondName) const
{
  auto it = bondPrices.find(bondName);
  if (it != bondPrices.end())
    return it->second;
  return 100.0; // fallback if not found
}

double Market::getStockPrice(const std::string &stockName) const
{
  auto it = stockPrices.find(stockName);
  if (it != stockPrices.end())
    return it->second;
  return 0.0;
}

const VolCurve &Market::getVolCurve(const std::string &name) const
{
  auto it = vols.find(name);
  if (it != vols.end())
    return it->second;
  throw std::runtime_error("VolCurve not found for " + name);
}

std::ostream &operator<<(std::ostream &os, const Market &mkt)
{
  os << mkt.asOf << std::endl;
  return os;
}

std::istream &operator>>(std::istream &is, Market &mkt)
{
  is >> mkt.asOf;
  return is;
}
