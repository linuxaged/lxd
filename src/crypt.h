#include "defines.h"
#include <vector>

namespace lxd {
	enum HashAlgorithm {
		SHA256,
		HMAC
	};
	DLL_PUBLIC std::vector<unsigned char> hash(HashAlgorithm algorithm, const char* message, size_t msgSize, const char* key = nullptr, size_t keySize = 0);
}