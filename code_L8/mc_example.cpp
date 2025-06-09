#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <numeric>
#include <chrono>
#include <future>
#include <vector>

using namespace std;

double computeMean(vector<double>& list) 
{
	double sum = accumulate(std::begin(list), std::end(list), 0.0);
	return sum/list.size();
};

double computeStdev(vector<double>& list) 
{
	double sum_sq = 0;
	double m = computeMean(list);
	std::for_each (std::begin(list), std::end(list), [&](const double d) {
    	sum_sq += (d - m) * (d - m);
	});
	return sqrt(sum_sq/(list.size()-1));
};

void normal_dist_gen(double mean, double stdev, vector<double>& list) 
{
	// Seed with a real random value, if available
    std::random_device rd;
	int path = list.size();
 
    // Generate a normal distribution around that mean
    std::mt19937 gen(rd());
    std::normal_distribution<>normal_dist(mean, stdev);
 
    for (int n = 0; n < path; ++n)
        list[n] = normal_dist(gen);

	//checks
	cout << "number of path: " << path <<endl;
	cout<< "mean of the vector: " << computeMean(list) << ";\t stdev of vector: " << computeStdev(list) << endl;

};

double path_pv(double z) 
{
	double Xt = log(100) + (0.05 - 0.02 - 0.5 * (0.2 * 0.2)) * 1 + 0.2*z;
	double St = exp(Xt);
	double payoff = max(St - 100, 0.);
	return payoff * exp(-1*0.05);
};

double path_pv2(vector<double> list) 
{	
	double pv = 0;
	for (size_t i =0 ; i < list.size(); i++ ) {
		double Xt = log(100) + (0.05 - 0.02 - 0.5 * (0.2 * 0.2)) * 1 + 0.2*list[i];
		double St = exp(Xt);
		double payoff = max(St - 100, 0.);
		pv+= payoff * exp(-1*0.05);
	}
	return pv;
}

int main()
{
    double mean = 0;
	double stdev = 1;
	int path = 100000;
	std::vector<double> list(path);
    normal_dist_gen(mean, stdev, list);
	
	/*
		price an european call with tau = 1, r = 5%, s = 100, k = 105, d = 2%, vol = 20%
		Geometric brownian motion
		dS/S = r*dt + r*sigma*dWt
		X = Ln(S)
		dXt = (r-d-0.5*sigam^2)*dt + sigma * dWt
	*/

	auto start = std::chrono::high_resolution_clock::now();
	double pv = 0;
	for (size_t i =0 ; i < list.size(); i++ ){
		double Xt = log(100) + (0.05 - 0.02 - 0.5 * (0.2 * 0.2)) * 1 + 0.2*list[i];
		double St = exp(Xt);
		double payoff = max(St - 100, 0.);
		pv += payoff * exp(-1*0.05);
	}
	pv = pv / path;
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

	cout << "Pv of the option is: " << pv <<endl;
	cout << "time taken: " << duration.count() << "ms." << endl;

	//example of using async for path pricing
	if (false) {
		auto start1 = std::chrono::high_resolution_clock::now();
		double pv = 0;
		vector<std::future<double>> _futures;
		for (size_t i =0 ; i < list.size(); i++ ){
			_futures.push_back(std::async(std::launch::async, path_pv, list[i])); 
		}
		for (auto && fut: _futures) {
            pv += fut.get();
		}
		pv = pv / path;
		auto stop1 = std::chrono::high_resolution_clock::now();
		auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(stop1 - start1);

		cout << "Pv of the option is: " << pv <<endl;
		cout << "time taken for aysnc function: " << duration1.count() << "ms." << endl;
	}

	//example of using async for path pricing
	if (true) {
		const auto processor_count = std::thread::hardware_concurrency();
		cout << "cpu count: " << processor_count << endl;
		auto start2 = std::chrono::high_resolution_clock::now();
		double pv = 0;
		vector<std::future<double>> _futures;
		int chunk_size = processor_count-2;
		auto iter = list.begin();
		int step = int(list.size() / chunk_size);
		while (iter <= list.end()) {
			auto end = iter + step;
			if (end >=list.end())
				end = list.end();
			vector<double>chunk(iter, end);
			_futures.push_back(std::async(std::launch::async, path_pv2, chunk));
			iter+=step;
		}
		
		for (auto && fut: _futures) {
            pv += fut.get();
		}
		pv = pv / path;
		auto stop2 = std::chrono::high_resolution_clock::now();
		auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(stop2 - start2);

		cout << "Pv of the option is: " << pv <<endl;
		cout << "time taken for aysnc function2: " << duration2.count() << "ms." << endl;
	}

}