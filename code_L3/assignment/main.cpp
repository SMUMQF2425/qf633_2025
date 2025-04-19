#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include "black.h"

using namespace std;

struct OptionTrade {
    double notional;
    double strike;
    double expiry;
    bool isCall;
};

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

void loadTradeFromFile(vector<OptionTrade>& tradesSet, const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        throw runtime_error("Could not open file: " + filePath);
    }

    string line;
    getline(file, line); // Skip header
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        vector<string> fields = split(line, ';');
        if (fields.size() < 5) {
            cerr << "Skipping invalid line: " << line << endl;
            continue;
        }

        try {
            // Parse notional with 'k' suffix
            string notionalStr = fields[1];
            double notional = 0.0;
            if (!notionalStr.empty() && (notionalStr.back() == 'k' || notionalStr.back() == 'K')) {
                notional = stod(notionalStr.substr(0, notionalStr.size()-1)) * 1000;
            } else {
                notional = stod(notionalStr);
            }

            OptionTrade trade{
                notional,
                stod(fields[2]),
                stod(fields[4]),
                (fields[3] == "true" || fields[3] == "1")
            };
            tradesSet.push_back(trade);
        } catch (const exception& e) {
            cerr << "Error parsing line: " << line << " - " << e.what() << endl;
        }
    }
}

void saveResultsToFile(const vector<double>& pvResults, const string& filePath) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw runtime_error("Could not open output file: " + filePath);
    }
    
    outFile << "TradeIndex;PV" << endl;
    for (size_t i = 0; i < pvResults.size(); ++i) {
        outFile << (i+1) << ";" << pvResults[i] << endl;
    }
}

int main() {
    cout << "Compute option PV task started." << endl;
    
    try {
        vector<OptionTrade> trades;
        loadTradeFromFile(trades, "trades.txt");
        
        vector<double> pvResults;
        const double spot = 100, vol = 0.2, rate = 0.045;
        
        for (const auto& trade : trades) {
            try {
                double pv = BlackScholes(
                    trade.notional, trade.strike, trade.expiry,
                    spot, vol, rate, trade.isCall
                );
                pvResults.push_back(pv);
            } catch (const exception& e) {
                cerr << "PV calculation failed: " << e.what() << endl;
                pvResults.push_back(0.0);
            }
        }
        
        saveResultsToFile(pvResults, "result.txt");
        cout << "Results saved to result.txt" << endl;
        
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    
    cout << "Compute option PV task completed." << endl;
    return 0;
}