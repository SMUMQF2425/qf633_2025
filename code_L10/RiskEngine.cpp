#include "RiskEngine.h"
#include "Pricer.h"

void RiskEngine::computeRisk(string riskType, shared_ptr<Trade> trade, bool singleThread)
{
    result.clear();
    auto pricer = CRRBinomialTreePricer(50); // or use your pricer pointer

    if (singleThread) {
        if (riskType == "dv01") {
            for (auto& kv : curveShocks) {
                string market_id = kv.first;
                auto mkt_u = kv.second.getMarketUp();
                auto mkt_d = kv.second.getMarketDown();
                double pv_up, pv_down;
                if (trade->getType() == "TreeProduct") {
                    pv_up = pricer.Price(mkt_u, trade);
                    pv_down = pricer.Price(mkt_d, trade);
                } else {
                    pv_up = trade->Pv(mkt_u);
                    pv_down = trade->Pv(mkt_d);
                }
                double dv01 = (pv_up - pv_down) / 2.0;
                result.emplace(market_id, dv01);
            }
        }

        if (riskType == "vega") {
            for (auto& kv : volShocks) {
                string market_id = kv.first;
                auto mkt = kv.second.getOriginMarket();
                auto mkt_s = kv.second.getMarket();
                double pv, pv_up;
                if (trade->getType() == "TreeProduct") {
                    pv = pricer.Price(mkt, trade);
                    pv_up = pricer.Price(mkt_s, trade);
                } else {
                    pv = trade->Pv(mkt);
                    pv_up = trade->Pv(mkt_s);
                }
                double vega = (pv_up - pv);
                result.emplace(market_id, vega);
            }
        }

        if (riskType == "price") {
            // to be added

        }
    }
    else {
        auto pv_task = [](shared_ptr<Trade> trade, string id, const Market& mkt_up, const Market& mkt_down) {
            double pv_up = trade->Pv(mkt_up);
            double pv_down = trade->Pv(mkt_down);
            double risk = (pv_up - pv_down) / 2.0;
            return std::make_pair(id, risk);
            };

        vector<std::future<std::pair<string, double>>> _futures;
        // calling the above function asynchronously and storing the result in future object
        for (auto& shock : curveShocks) {
            string market_id = shock.first;
            auto mkt_u = shock.second.getMarketUp();
            auto mkt_d = shock.second.getMarketDown();
            _futures.push_back(std::async(std::launch::async, pv_task, trade, market_id, mkt_u, mkt_d));
        }

        for (auto& shock : volShocks) {
            string market_id = shock.first;
            auto mkt = shock.second.getOriginMarket();
            auto mkt_s = shock.second.getMarket();
            _futures.push_back(std::async(std::launch::async, pv_task, trade, market_id, mkt, mkt_s));
        }

        for (auto&& fut : _futures) {
            auto rs = fut.get();
            result.emplace(rs);
        }

    }
}