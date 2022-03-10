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
	ifstream sourceIn(argv[1], ios::binary);
	if (!sourceIn.is_open()) {
		cerr << "File" << argv[1] << " could not have been opened" << endl;
		cin.get();
		return 1;
	}
	ifstream decoderIn(argv[2], ios::binary);
	if (!decoderIn.is_open()) {
		cerr << "File" << argv[2] << " could not have been opened" << endl;
		cin.get();
		return 1;
	}
	long long* N = 0;
	char num[8]{ 0 };
	decoderIn.read(num, 8 * sizeof(char));
	N = reinterpret_cast<long long*>(num);
	sourceIn.read(num, 8 * sizeof(char));
	unsigned long long errCount = 0;
	cout << "Processing... \n";
	{
		char bufferSource[16]{ 0 }, bufferDecoder[16]{ 0 };
		for (auto i = 0ll; i < (*N / 8 + 1) / 16; ++i) {
			sourceIn.read(bufferSource, 16);
			decoderIn.read(bufferDecoder, 16);
			for (auto j = 0u; j < 16; ++j) {
				if (bufferSource[j] != bufferDecoder[j]) {
					for (unsigned char mask = 0b10000000, cnt = 0; cnt < 8; ++cnt) {
						if ((mask & bufferSource[j]) !=  (mask & bufferDecoder[j])) {
							cout << "Error at position " << i * 16 * 8 + j * 8 + cnt << '\n';
							++errCount;
						}
						mask >>= 1;
					}
				}
			}
		}
		if ((*N / 8 + 1) % 16) {
			auto left = (*N / 8 + 1) % 16;
			sourceIn.read(bufferSource, left);
			decoderIn.read(bufferDecoder, left);
			for (auto j = 0u; j < left; ++j) {
				if (bufferSource[j] != bufferDecoder[j]) {
					for (unsigned char mask = 0b10000000, cnt = 0; cnt < 8; ++cnt) {
						if ((mask & bufferSource[j]) !=  (mask & bufferDecoder[j])) {
							auto pos = (*N / 8 + 1) / 16 * 8 + j * 8 + cnt;
							if (pos >= *N) {
								mask >>= 1;
								continue;
							}
							cout << "Error at position " << pos << '\n';
							++errCount;
						}
						mask >>= 1;
					}
				}
			}
		}
	}
	cout << *N << " bits processed successfully\n" << errCount << " errors detected" << endl;

	cin.get();
	return 0;
}
