#include "Bond.h"
#include "Market.h"

void Bond::generateSchedule()
{
  //implement this
	if (startDate == maturityDate || frequency <= 0 || frequency > 1)
		throw std::runtime_error("Error: start date is later than end date, or invalid frequency!");

	string tenorStr;
	if (frequency == 0.25)
		tenorStr = "3M";
	else if (frequency == 0.5)
		tenorStr = "6M";
	else
		tenorStr = "1Y";

	Date seed = startDate;
	while (seed < maturityDate) {
		bondSchedule.push_back(seed);
		seed = dateAddTenor(seed, tenorStr);
	}
	bondSchedule.push_back(maturityDate);
	if (bondSchedule.size() < 2)
		throw std::runtime_error("Error: invalid schedule, check input!");

}
double Bond::Payoff(double s) const
{
  double pv = notional * (s - tradePrice);
  return pv;
}
double Bond::Pv(const Market& mkt) const
{
    // Using cash flow discounting
    double pv = 0.0;
    Date valueDate = mkt.asOf;
    auto rc = mkt.getCurve(rateCurve);
    int sign = (notional >= 0) ? 1 : -1; // long or short
    for (size_t i = 1; i < bondSchedule.size(); i++) {
        auto dt = bondSchedule[i];
        if (dt < valueDate)
            continue;
        double tau = (bondSchedule[i] - bondSchedule[i - 1]) / 360.0;
        double df = rc->getDf(dt);
        // Coupon payment
        pv += sign * notional * coupon * tau * df;
    }
    // Principal repayment at maturity
    double dfMat = rc->getDf(maturityDate);
    pv += sign * notional * dfMat;
    return pv;
}



