#include <iostream>
#include <fstream>
#include <cstdlib>
#include <immintrin.h>

using namespace std;

char* extractBitsToChar(long long* amount, const char* filename) {
	ifstream in(filename, ios::binary);
	if (!in.is_open()) {
		cerr << "File " << filename << " could not have been opened" << endl;
		return nullptr;
	}
	cout << "Extracting bits... ";
	long long N = 0;
	char num[8]{ 0 };
	in.read(num, 8 * sizeof(char));
	for (int i = 7; i >= 0; --i) {
		long long temp = 0;
		temp |= static_cast<unsigned char>(num[i]);
		temp <<= i * 8;
		N |= temp;
	}
	char* bits = new char[N];
	char* buffer = new char[N / 8 + 1];
	long cnt = 0;
	in.read(buffer, (N / 8 + 1) * sizeof(char));
	for (long long i = 0; i < N / 8; ++i) {
		unsigned char c = static_cast<unsigned char>(buffer[i]);
		for (long long j = 0; j < 8; ++j) {
			if (c & 0b10000000) bits[i * 8 + j] = '1';
			else bits[i * 8 + j] = '0';
			c = c << 1;
		}
	}
	unsigned char c = static_cast<unsigned char>(buffer[N / 8]);
	for (long long j = 0; j < N % 8; ++j) {
		if (c & 0b10000000) bits[N - N % 8 + j] = '1';
		else bits[N - N % 8 + j] = '0';
		c = c << 1;
	}
	cout << N << " bits successfully extracted" << endl;
	if (amount) *amount = N;
	in.close();
	delete[] buffer;
	return bits;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		cerr << "Invalid argument amount" << endl;
		return 1;
	}
	ofstream out(argv[2], ios::binary);
	if (!out.is_open()) {
		cerr << "File" << argv[2] << " could not have been opened" << endl;
		return 1;
	}
	long long N = 0;
	ifstream in(argv[1], ios::binary);
	char num[8]{ 0 };
	in.read(num, 8 * sizeof(char));
	for (int i = 7; i >= 0; --i) {
		long long temp = 0;
		temp |= static_cast<unsigned char>(num[i]);
		temp <<= i * 8;
		N |= temp;
	}
	for (long long i = 0; i < N / 56 + 1; ++i) {
		if (i == N / 56) {
			long long left = (N % 56) / 8 + 1;
			unsigned char* buffer = new unsigned char[left]{ 0 };
			char* bits = new char[left]{ 0 };
			in.read(bits, left);
			unsigned char c = 0b00000000;
			unsigned char temp = 0b00000000;
			unsigned short cnt = 0b10000000;
			for (auto j = 0u; j < left; ++j) {
				c = static_cast<unsigned char>(bits[j]);
				for (auto k = 0u; k < 4; ++k) {
					if (c & 0b10000000) buffer[k] = 1;
					else buffer[k] = 0;
					c <<= 1;
				}
				buffer[4] = buffer[0] ^ buffer[1] ^ buffer[2];
				buffer[5] = buffer[1] ^ buffer[2] ^ buffer[3];
				buffer[6] = buffer[0] ^ buffer[2] ^ buffer[3];
				for (auto k = 0u; k < 7; ++k) {
					if (buffer[k]) temp |= cnt;
					cnt >>= 1;
				}
				out.put(temp);
				cnt = 0b10000000;
				temp = 0b00000000;
				for (auto k = 0u; k < 4; ++k) {
					if (c & 0b10000000) buffer[k] = 1;
					else buffer[k] = 0;
					c <<= 1;
				}
				buffer[4] = buffer[0] ^ buffer[1] ^ buffer[2];
				buffer[5] = buffer[1] ^ buffer[2] ^ buffer[3];
				buffer[6] = buffer[0] ^ buffer[2] ^ buffer[3];
				for (auto k = 0u; k < 7; ++k) {
					if (buffer[k]) temp |= cnt;
					cnt >>= 1;
				}
				out.put(temp);
				cnt = 0b10000000;
				temp = 0b00000000;
			}
		}
		else {
			unsigned char buffer[8]{ 0 };
			char bits[8]{ 0 };
			in.read(bits, 8);
			unsigned char c = 0b00000000;
			unsigned char temp = 0b00000000;
			unsigned short cnt = 0b10000000;
			for (auto j = 0u; j < 8; ++j) {
				c = static_cast<unsigned char>(bits[j]);
				for (auto k = 0u; k < 4; ++k) {
					if (c & 0b10000000) buffer[k] = 1;
					else buffer[k] = 0;
					c <<= 1;
				}
				buffer[4] = buffer[0] ^ buffer[1] ^ buffer[2];
				buffer[5] = buffer[1] ^ buffer[2] ^ buffer[3];
				buffer[6] = buffer[0] ^ buffer[2] ^ buffer[3];
				for (auto k = 0u; k < 7; ++k) {
					if (buffer[k]) temp |= cnt;
					cnt >>= 1;
				}
				out.put(temp);
				cnt = 0b10000000;
				temp = 0b00000000;
				for (auto k = 0u; k < 4; ++k) {
					if (c & 0b10000000) buffer[k] = 1;
					else buffer[k] = 0;
					c <<= 1;
				}
				buffer[4] = buffer[0] ^ buffer[1] ^ buffer[2];
				buffer[5] = buffer[1] ^ buffer[2] ^ buffer[3];
				buffer[6] = buffer[0] ^ buffer[2] ^ buffer[3];
				for (auto k = 0u; k < 7; ++k) {
					if (buffer[k]) temp |= cnt;
					cnt >>= 1;
				}
				out.put(temp);
				cnt = 0b10000000;
				temp = 0b00000000;
			}
		}
	}
	cout << N << " bits successfully encoded and written to " << argv[1] << endl;
	out.close();
	in.close();

	char* bitties = extractBitsToChar(nullptr, argv[2]);
	for (auto i = 0; i < N; ++i) {
		cout << bitties[i];
	}
	cout << endl;
	return 0;
}
