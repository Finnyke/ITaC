#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

char* extractBitsToChar(long* amount, const char* filename) {
	ifstream in(filename, ios::binary);
	if (!in.is_open()) {
		cerr << "File " << filename << " could not have been opened" << endl;
	}
	cout << "Extracting bits... ";
	long N = 0;
	char num[4]{0};
	in.read(num, 4 * sizeof(char));
	N |= (static_cast<unsigned char>(num[3]) << 24);
	N |= (static_cast<unsigned char>(num[2]) << 16);
	N |= (static_cast<unsigned char>(num[1]) << 8);
	N |= (static_cast<unsigned char>(num[0]) << 0);
	char* bits = new char[N];
	char* buffer = new char[N / 8 + 1];
	long cnt = 0;
	in.read(buffer, (N / 8 + 1) * sizeof(char));
	for (long i = 0; i < N / 8; ++i) {
		unsigned char c = static_cast<unsigned char>(buffer[i]);
		for (long j = 0; j < 8; ++j) {
			if (c & 0b10000000) bits[i * 8 + j] = '1';
			else bits[i * 8 + j] = '0';
			c = c << 1;
		}
	}
	unsigned char c = static_cast<unsigned char>(buffer[N / 8]);
	for (long j = 0; j < N % 8; ++j) {
		if (c & 0b10000000) bits[N - N % 8 + j] = '1';
		else bits[N - N % 8 + j] = '0';
		c = c << 1;
	}
	cout << N << " bits successfully extracted" << endl;
	delete[] buffer;
	if (amount) *amount = N;
	return bits;
}

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
	cout << "Writing... ";
	srand(static_cast<unsigned>(time(nullptr)));
	unsigned char buffer = 0b00000000;
	unsigned short cnt = 0;
	out.write(reinterpret_cast<const char*>(&N), sizeof(N));
	if (p < 1 && p > 0) {
		for (long i = 0; i < N + 8; ++i) {
			double r = round(((double)rand() / (RAND_MAX)) - 0.5 + p);
			++cnt;
			if (r) buffer |= (1 << (8 - cnt));
			if (cnt == 8) {
				cnt = 0;
				out.put(buffer);
				buffer = 0b00000000;
			}
		}
	}
	else {
		p ? buffer = 0b11111111 : buffer = 0b00000000;
		for (long i = 0; i < N / 8 + 1; ++i) {
			out.put(buffer);
		}
	}
	cout << N << " bits successfully written to " << argv[1] << endl;
	out.close();

	return 0;
}
