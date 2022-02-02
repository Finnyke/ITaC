#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
	if (argc != 3 && argc != 4) {
		cerr << "Invalid argument amount" << endl;
		return 1;
	}
	ofstream out(argv[1], ios::binary);
	if (!out.is_open()) {
		cerr << "File" << argv[1] << " could not have been opened" << endl;
		return 1;
	}
	long N;
	N = atol(argv[2]);
	if (N <= 0) {
		cerr << "Invalid number of bits" << endl;
		return 1;
	}
	double p;
	if (argc == 4) {
		p = atof(argv[3]);
		if (p < 0 || p > 1) {
			cerr << "Invalid probability" << endl;
			return 1;
		}
	}
	else p = 0.5;
	srand(static_cast<unsigned>(time(nullptr)));
	if (p < 1 && p > 0) {
		for (long i = 0; i < N; ++i) {
			double r = round(((double)rand() / (RAND_MAX)) - 0.5 + p);
			char bit;
			r ? bit = 1 : bit = 0;
			out.put(bit + 48);
		}
	}
	else {
		for (long i = 0; i < N; ++i) {
			char bit;
			p ? bit = 1 : bit = 0;
			out.put(bit + 48);
		}
	}
	cout << N << " bits written to " << argv[1] << endl;
	out.close();

	return 0;
}