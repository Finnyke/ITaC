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
	long long N;
	N = atoll(argv[2]);
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
	cout << "Writing... ";
	srand(static_cast<unsigned>(time(nullptr)));
	out.write(reinterpret_cast<const char*>(&N), sizeof(N));
	if (p < 1 && p > 0) {
		unsigned char buffer = 0b00000000;
		unsigned short cnt = 0b10000000;
		for (long long i = 0; i < N + 8 - N % 8; ++i) {
			if (rand() < RAND_MAX * p) buffer |= cnt;
			cnt >>= 1;
			if (!cnt) {
				cnt = 0b10000000;
				out.put(buffer);
				buffer = 0b00000000;
			}
		}
	}
	else {
		unsigned char buffer;
		p ? buffer = 0b11111111 : buffer = 0b00000000;
		for (long long i = 0; i < N / 8 + 1; ++i) {
			out.put(buffer);
		}
	}
	cout << N << " bits successfully written to " << argv[1] << endl;
	out.close();

	return 0;
}
