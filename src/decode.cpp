#include <iostream>

#include "audiofile/AudioFile.h"

#include "ZeroCrossingDetector.h"
#include "BitDetector.h"
#include "ByteDetector.h"

using namespace std;

auto main(int argc, char** argv) -> int {
	if(argc < 3) {
		cout << "usage: decode <wav file> <rectangle width max variation> [--raw]" << endl;
		return 0;
	}
	bool printRaw = false;

	const std::string fileName = argv[1];
	const double maxWidthVariation = stod(argv[2]);
	if(maxWidthVariation < 0.0 || maxWidthVariation > 1.0) {
		cout << "max rectangle width variation must be >= 0.0 and <= 1.0" << endl;
		return 0;
	}

	if(argc >= 4 && string(argv[3]) == "--raw") {
		printRaw = true;
	}

	cout << "loading " << fileName << endl;

	AudioFile<double> audioFile;
	audioFile.load(fileName);

	cout << "loaded file" << endl;
	audioFile.printSummary();

	for(int channel = 0; channel < audioFile.getNumChannels(); channel++)
	{
		cout << "parsing channel " << channel << endl;

		cout << "detecting zero crossings" << endl;
		ZeroCrossingDetector zcDetector;
		Crossings crossings;
		zcDetector.detect(audioFile.samples[channel], back_inserter(crossings));
		cout << "detected " << crossings.size() << " zero-crossings " << endl;

		cout << "detecting bits" << endl;
		BitDetector bitDetector;
		Bits bits;
		BitDetector::Errors bitErrors;
		BitDetector::Segments segments;

		auto result = bitDetector.detect(
			audioFile.samples[channel].size(),
			crossings,
			audioFile.getSampleRate(),
			maxWidthVariation,
			back_inserter(bits),
			back_inserter(bitErrors),
			back_inserter(segments)
		);

		// for(const auto& s : segments) {
		// 	cout << "segment " << s.length << endl;
		// }

		cout << "detected " << bits.size() << " bits" << endl;
		cout << result.zeroBits << " zeros, " << result.oneBits << " ones" << endl;
		cout << bitErrors.size() << " errors " << endl;
		for(const auto& e: bitErrors) {
			cout << "bit error: " << e.message << " at segment: " << e.segmentId << endl;
		}

		if(printRaw) {
			cout << "bits: " << endl;
			for(const auto& b: bits) {
				cout << to_string(b);
			}
			cout << endl;
		}

		cout << "detecting bytes" << endl;
		ByteDetector byteDetector;
		Bytes bytes;
		ByteDetector::Errors byteErrors;

		byteDetector.detect(
			bits,
			back_inserter(bytes),
			back_inserter(byteErrors)
		);

		cout << "detected " << bytes.size() << " bytes, " << byteErrors.size() << " errors " << endl;
		for(const auto& e: byteErrors) {
			cout << "byte error: " << e.message << " at bit: " << e.position << endl;
			cout << "bit context: " << e.bitContext << endl;
		}

		if(printRaw) {
			cout << "bytes: " << endl;
			for(const auto& b : bytes) {
				cout << hex << static_cast<int>(b) << dec << " ";
			}
			cout << endl;
		}
	}
	return 0;
}