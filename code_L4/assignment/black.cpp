#include <cmath>
#include "black.h"

static double norm_cdf(double x) {
    return 0.5 * (1.0 + std::erf(x / std::sqrt(2.0)));
}

double BlackScholes(double notional, double strike, double expiry, 
                   double spot, double vol, double rate, bool isCall) {
    if (expiry <= 0 || vol <= 0 || spot <= 0 || strike <= 0) {
        return 0.0;
    }

    double d1 = (std::log(spot / strike) + (rate + 0.5 * vol * vol) * expiry) 
                / (vol * std::sqrt(expiry));
    double d2 = d1 - vol * std::sqrt(expiry);
    double discount = strike * std::exp(-rate * expiry);

    if (isCall) {
        return notional * (spot * norm_cdf(d1) - discount * norm_cdf(d2));
    }
    return notional * (discount * norm_cdf(-d2) - spot * norm_cdf(-d1));
}