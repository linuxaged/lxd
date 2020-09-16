#include "base64.h"
#include <cstdint>

namespace lxd {
	// alphabet character for a radix-64 number
	const char base64_alphabet[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"	// [ 0, 25]
		"abcdefghijklmnopqrstuvwxyz"	// [26, 51]
		"0123456789"					// [52, 61]
		"+/";							// [62, 63]

	// radix-64 number for an alphabet character
	const char base64_radix64[] =
	{
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62,  0,  0,  0, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,
		 0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0,  0,
		 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
	};

	size_t Base64_EncodeSizeInBytes(size_t dataSizeInBytes) {
		return (dataSizeInBytes + 2) / 3 * 4;
	}

	size_t Base64_DecodeSizeInBytes(const char* base64, const size_t base64SizeInBytes) {
		int padding = 0;
		for(size_t i = base64SizeInBytes - 1; i > 0 && base64[i] == '='; i--) {
			padding++;
		}
		return ((3 * base64SizeInBytes + 3) / 4) - padding;
	}

	size_t Base64_Encode(char* base64, const unsigned char* data, const size_t dataSizeInBytes) {
		size_t base64SizeInBytes = 0;
		size_t byteCount = 0;
		unsigned char bytes[3];

		for(size_t i = 0; i < dataSizeInBytes; i++) {
			bytes[byteCount++] = data[i];
			if(byteCount == 3 || i == dataSizeInBytes - 1) {
				// Pad the input data with zeros up to three bytes.
				for(size_t j = byteCount; j < 3; j++) {
					bytes[j] = 0;
				}

				// Convert from three bytes to four radix-64 numbers.
				unsigned char radix64[4];
				radix64[0] = (unsigned char)(((bytes[0] & 0xFC) >> 2));
				radix64[1] = (unsigned char)(((bytes[0] & 0x03) << 4) + ((bytes[1] & 0xF0) >> 4));
				radix64[2] = (unsigned char)(((bytes[1] & 0x0F) << 2) + ((bytes[2] & 0xC0) >> 6));
				radix64[3] = (unsigned char)(((bytes[2] & 0x3F) >> 0));

				// Convert from radix-64 to the base64 alphabet.
				for(size_t j = 0; j < byteCount + 1; j++) {
					base64[base64SizeInBytes++] = base64_alphabet[radix64[j]];
				}

				// Pad the base64 data with '='.
				for(size_t j = byteCount + 1; j < 4; j++) {
					base64[base64SizeInBytes++] = '=';
				}
				byteCount = 0;
			}
		}
		return base64SizeInBytes;
	}

	size_t Base64_Decode(unsigned char* data, const char* base64, const size_t base64SizeInBytes, const size_t maxDecodeSizeInBytes) {
		size_t maxDecodeBytes = (maxDecodeSizeInBytes > 0) ? maxDecodeSizeInBytes : SIZE_MAX;
		size_t dataSizeInBytes = 0;
		size_t alphabetCount = 0;
		unsigned char alphabet[4];

		for(size_t i = 0; i < base64SizeInBytes && dataSizeInBytes < maxDecodeBytes; i++) {
			alphabet[alphabetCount++] = base64[i];
			if(alphabetCount == 4 || i == base64SizeInBytes - 1) {
				// Pad the base64 data with '=' in case there is no padding.
				for(size_t j = alphabetCount; j < 4; j++) {
					alphabet[j] = '=';
				}

				// Don't consider any stored padding as alphabet characters.
				while(alphabetCount > 0 && alphabet[alphabetCount - 1] == '=') {
					alphabetCount--;
				}

				// Convert from the base64 alphabet to radix-64.
				char radix64[4];
				for(size_t j = 0; j < 4; j++) {
					radix64[j] = base64_radix64[alphabet[j]];
				}

				// Convert from four radix-64 numbers to three bytes.
				unsigned char bytes[3];
				bytes[0] = (unsigned char)(((radix64[0] & 0xFF) << 2) + ((radix64[1] & 0x30) >> 4));
				bytes[1] = (unsigned char)(((radix64[1] & 0x0F) << 4) + ((radix64[2] & 0x3C) >> 2));
				bytes[2] = (unsigned char)(((radix64[2] & 0x03) << 6) + radix64[3]);

				// Store output data.
				for(size_t j = 0; j < alphabetCount - 1 && dataSizeInBytes < maxDecodeBytes; j++) {
					data[dataSizeInBytes++] = bytes[j];
				}
				alphabetCount = 0;
			}
		}
		return dataSizeInBytes;
	}
}
