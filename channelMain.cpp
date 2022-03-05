#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
	if (argc != 3 && argc != 4) {
		cerr << "Invalid argument amount" << endl;
		cin.get();
		return 1;
	}
	ifstream in(argv[1], ios::binary);
	if (!in.is_open()) {
		cerr << "File " << argv[1] << " could not have been opened" << endl;
		cin.get();
		return 1;
	}
	ofstream out(argv[2], ios::binary);
	if (!out.is_open()) {
		cerr << "File " << argv[2] << " could not have been opened" << endl;
		cin.get();
		return 1;
	}
	double p;
	if (argc == 4) {
		p = atof(argv[3]);
		if (p < 0 || p > 1) {
			cerr << "Invalid probability" << endl;
			cin.get();
			return 1;
		}
	}
	else p = 0.0001;
	long long* N = 0;
	char num[8]{ 0 };
	in.read(num, 8 * sizeof(char));
	N = reinterpret_cast<long long*>(num);
	out.write(reinterpret_cast<const char*>(N), sizeof(*N));
	cout << "Processing... ";
	long long byteAmount = (*N / 8 + 1) * 7 / 4;
	if ((*N / 8 + 1) * 7 % 4) ++byteAmount;
	srand(static_cast<unsigned>(time(nullptr)));
	char buffer[16]{ 0 };
	unsigned char c = 0b10000000;
	for (auto i = 0ll; i < byteAmount / 16; ++i) {
		in.read(buffer, 16);
		for (auto j = 0u; j < 16 * 8; ++j) {
			if (rand() < RAND_MAX * p) buffer[j / 16] ^= (c >> (j % 8));
		}
		for (auto j = 0u; j < 16; ++j) {
			out.put(buffer[j]);
		}
	}
	if (byteAmount % 16) {
		in.read(buffer, byteAmount % 16);
		for (auto j = 0u; j < (byteAmount % 16) * 8; ++j) {
			if (rand() < RAND_MAX * p) buffer[j / 16] ^= (c >> (j % 8));
		}
		for (auto j = 0u; j < byteAmount % 16; ++j) {
			out.put(buffer[j]);
		}
	}
	cout << *N << " bits processed successfully" << endl;
	in.close();
	out.close();

	cin.get();
	return 0;
}
