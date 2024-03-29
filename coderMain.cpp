#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv) {
	if (argc != 3) {
		cerr << "Invalid argument amount" << endl;
		cin.get();
		return 1;
	}
	ifstream in(argv[1], ios::binary);
	if (!in.is_open()) {
		cerr << "File" << argv[1] << " could not have been opened" << endl;
		cin.get();
		return 1;
	}
	ofstream out(argv[2], ios::binary);
	if (!out.is_open()) {
		cerr << "File" << argv[2] << " could not have been opened" << endl;
		cin.get();
		return 1;
	}
	long long* N = 0;
	char num[8]{ 0 };
	in.read(num, 8 * sizeof(char));
	N = reinterpret_cast<long long*>(num);
	out.write(reinterpret_cast<const char*>(N), sizeof(*N));
	cout << "Encoding... ";
	{
		long long output;
		unsigned long long cnt;
		char buffer[7]{ 0 };
		unsigned char c;
		char bits[4]{ 0 };
		for (auto i = 0ll; i < (*N / 8 + 1) / 4; ++i) {
			output = 0;
			cnt = 0x8000000000000000;
			in.read(bits, 4);
			for (auto j = 0u; j < 4; ++j) {
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
					if (buffer[k]) output |= cnt;
					cnt >>= 1;
				}
				for (auto k = 0u; k < 4; ++k) {
					if (c & 0b10000000) buffer[k] = 1;
					else buffer[k] = 0;
					c <<= 1;
				}
				buffer[4] = buffer[0] ^ buffer[1] ^ buffer[2];
				buffer[5] = buffer[1] ^ buffer[2] ^ buffer[3];
				buffer[6] = buffer[0] ^ buffer[2] ^ buffer[3];
				for (auto k = 0u; k < 7; ++k) {
					if (buffer[k]) output |= cnt;
					cnt >>= 1;
				}
			}
			const char* buf = reinterpret_cast<const char*>(&output);
			for (auto j = 1u; j < 8; ++j) {
				out.put(buf[j]);
			}
		}
		if ((*N / 8 + 1) % 4) {
			auto left = (*N / 8 + 1) % 4;
			output = 0;
			cnt = 0x8000000000000000;
			in.read(bits, left);
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
					if (buffer[k]) output |= cnt;
					cnt >>= 1;
				}
				for (auto k = 0u; k < 4; ++k) {
					if (c & 0b10000000) buffer[k] = 1;
					else buffer[k] = 0;
					c <<= 1;
				}
				buffer[4] = buffer[0] ^ buffer[1] ^ buffer[2];
				buffer[5] = buffer[1] ^ buffer[2] ^ buffer[3];
				buffer[6] = buffer[0] ^ buffer[2] ^ buffer[3];
				for (auto k = 0u; k < 7; ++k) {
					if (buffer[k]) output |= cnt;
					cnt >>= 1;
				}
			}
			const char* buf = reinterpret_cast<const char*>(&output);
			for (auto j = (8 - 2 * left); j < 8; ++j) {
				out.put(buf[j]);
			}
		}
	}
	cout << "Encoding completed successfully" << endl;
	in.close();
	out.close();

	cin.get();
	return 0;
}
