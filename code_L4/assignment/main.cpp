#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include "black.h"
#include "trade.h"

using namespace std;

struct PvResult {
    string trade_id;
    string trade_info;
    double pv;
};

vector<string> split(const string& str, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0, end;
    while ((end = str.find(delimiter, start)) != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

void loadTradeFromFile(std::vector<OptionTrade*>& tradesSet, const std::string& filename) {
    ifstream input_file(filename);
    if (!input_file.is_open()) throw runtime_error("Cannot open file: " + filename);

    string line;
    getline(input_file, line); // Skip header
    
    while (getline(input_file, line)) {
        if (line.empty()) continue;
        vector<string> fields = split(line, ";");
        if (fields.size() < 6) continue;

        try {
            // Parse notional (handle 'k' suffix)
            string notionalStr = fields[1];
            double notional = 0;
            if (!notionalStr.empty() && (notionalStr.back() == 'k' || notionalStr.back() == 'K')) {
                notional = stod(notionalStr.substr(0, notionalStr.size()-1)) * 1000;
            } else {
                notional = stod(notionalStr);
            }

            // Parse dates (remove quotes)
            std::string startDate = fields[4].substr(1, 10);  // Extract "2021-12-23"
            std::string endDate = fields[5].substr(1, 10);    // Extract "2022-06-23"
            
            tradesSet.push_back(new OptionTrade(
                notional,
                stod(fields[2]),
                (fields[3] == "true"),
                startDate,
                endDate
            ));
        } catch (...) {
            cerr << "Error processing line: " << line << endl;
        }
    }
}

void writeTofile(const vector<PvResult>& results, const string& filename) {
    ofstream outfile(filename);
    if (!outfile) throw runtime_error("Cannot open output file");

    outfile << left << setw(12) << "Trade ID" << setw(35) << "Trade Info" << "Present Value\n";
    outfile << string(60, '-') << "\n";
    
    for (const auto& res : results) {
        outfile << setw(12) << res.trade_id 
                << setw(35) << res.trade_info 
                << fixed << setprecision(2) << res.pv << "\n";
    }
}

int main() {
    vector<OptionTrade*> tradesSet;
    vector<PvResult> results;
    const double spot = 100, vol = 0.2, rate = 0.045;

    try {
        loadTradeFromFile(tradesSet, "trades.txt");
        
        for (size_t i = 0; i < tradesSet.size(); ++i) {
            try {
                double pv = tradesSet[i]->calculatePv(spot, vol, rate);
                results.push_back({
                    to_string(i+1),
                    tradesSet[i]->getTradeDetails(),
                    pv
                });
            } catch (...) {
                cerr << "Error calculating PV for trade " << (i+1) << endl;
            }
        }

        writeTofile(results, "results.txt");
        cout << "Successfully processed " << tradesSet.size() << " trades\n";

    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
    }

    // Cleanup
    for (auto& trade : tradesSet) delete trade;
    return 0;
}