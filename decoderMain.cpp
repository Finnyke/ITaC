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
	long long* N = 0;
	char num[8]{ 0 };
	in.read(num, 8 * sizeof(char));
	N = reinterpret_cast<long long*>(num);
	out.write(reinterpret_cast<const char*>(N), sizeof(*N));
	cout << "Decoding... ";
	{
		long long byteAmount = (*N / 8 + 1) * 7 / 4;
		if ((*N / 8 + 1) * 7 % 4) ++byteAmount;
		long long* inputptr;
		char buffer[8]{ 0 };
		char bits[8]{ 0 };
		long long input;
		unsigned long long mask = 0x0080000000000000;
		unsigned char c;
		unsigned char carr[4]{ 0 };
		unsigned char syndrome;
		for (auto i = 0ll; i < byteAmount / 7; ++i) {
			in.read(buffer, 7);
			inputptr = reinterpret_cast<long long*>(buffer);
			input = *inputptr;
			for (auto j = 0u; j < 4; ++j) {
				c = 0;
				syndrome = 0;
				for (auto k = 0u; k < 7; ++k) {
					if (input & mask) bits[k] = 1;
					else bits[k] = 0;
					input <<= 1;
				}
				syndrome |= bits[4] ^ bits[0] ^ bits[1] ^ bits[2];
				syndrome <<= 1;
				syndrome |= bits[5] ^ bits[1] ^ bits[2] ^ bits[3];
				syndrome <<= 1;
				syndrome |= bits[6] ^ bits[0] ^ bits[2] ^ bits[3];
				switch (syndrome) {
				case 0b00000000:
					break;
				case 0b00000101:
					bits[0] ^= 1;
					break;
				case 0b00000110:
					bits[1] ^= 1;
					break;
				case 0b00000111:
					bits[2] ^= 1;
					break;
				case 0b00000011:
					bits[3] ^= 1;
					break;
				default:
					break;
				}
				for (auto k = 0u; k < 4; ++k) {
					c <<= 1;
					c |= bits[k];
				}
				syndrome = 0;
				for (auto k = 0u; k < 7; ++k) {
					if (input & mask) bits[k] = 1;
					else bits[k] = 0;
					input <<= 1;
				}
				syndrome |= bits[4] ^ bits[0] ^ bits[1] ^ bits[2];
				syndrome <<= 1;
				syndrome |= bits[5] ^ bits[1] ^ bits[2] ^ bits[3];
				syndrome <<= 1;
				syndrome |= bits[6] ^ bits[0] ^ bits[2] ^ bits[3];
				switch (syndrome) {
				case 0b00000000:
					break;
				case 0b00000101:
					bits[0] ^= 1;
					break;
				case 0b00000110:
					bits[1] ^= 1;
					break;
				case 0b00000111:
					bits[2] ^= 1;
					break;
				case 0b00000011:
					bits[3] ^= 1;
					break;
				default:
					break;
				}
				for (auto k = 0u; k < 4; ++k) {
					c <<= 1;
					c |= bits[k];
				}
				carr[j] = c;
			}
			for (auto j = 0u; j < 4; ++j) {
				out.put(carr[j]);
			}
		}
		if (byteAmount % 7) {
			auto left = byteAmount % 7;
			mask >>= (7 - left) * 8;
			in.read(buffer, left);
			inputptr = reinterpret_cast<long long*>(buffer);
			input = *inputptr;
			for (auto j = 0u; j < left * 4 / 7; ++j) {
				c = 0;
				syndrome = 0;
				for (auto k = 0u; k < 7; ++k) {
					if (input & mask) bits[k] = 1;
					else bits[k] = 0;
					input <<= 1;
				}
				syndrome |= bits[4] ^ bits[0] ^ bits[1] ^ bits[2];
				syndrome <<= 1;
				syndrome |= bits[5] ^ bits[1] ^ bits[2] ^ bits[3];
				syndrome <<= 1;
				syndrome |= bits[6] ^ bits[0] ^ bits[2] ^ bits[3];
				switch (syndrome) {
				case 0b00000000:
					break;
				case 0b00000101:
					bits[0] ^= 1;
					break;
				case 0b00000110:
					bits[1] ^= 1;
					break;
				case 0b00000111:
					bits[2] ^= 1;
					break;
				case 0b00000011:
					bits[3] ^= 1;
					break;
				default:
					break;
				}
				for (auto k = 0u; k < 4; ++k) {
					c <<= 1;
					c |= bits[k];
				}
				syndrome = 0;
				for (auto k = 0u; k < 7; ++k) {
					if (input & mask) bits[k] = 1;
					else bits[k] = 0;
					input <<= 1;
				}
				syndrome |= bits[4] ^ bits[0] ^ bits[1] ^ bits[2];
				syndrome <<= 1;
				syndrome |= bits[5] ^ bits[1] ^ bits[2] ^ bits[3];
				syndrome <<= 1;
				syndrome |= bits[6] ^ bits[0] ^ bits[2] ^ bits[3];
				switch (syndrome) {
				case 0b00000000:
					break;
				case 0b00000101:
					bits[0] ^= 1;
					break;
				case 0b00000110:
					bits[1] ^= 1;
					break;
				case 0b00000111:
					bits[2] ^= 1;
					break;
				case 0b00000011:
					bits[3] ^= 1;
					break;
				default:
					break;
				}
				for (auto k = 0u; k < 4; ++k) {
					c <<= 1;
					c |= bits[k];
				}
				carr[j] = c;
			}
			for (auto j = 0u; j < left * 4 / 7; ++j) {
				out.put(carr[j]);
			}
		}
	}
	cout << "Decoding completed successfully" << endl;

	cin.get();
	return 0;
}
