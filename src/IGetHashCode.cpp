#include "IGetHashCode.hpp"

namespace vwr {
    int32_t GetHashCode(const size_t value)
    {
        uint64_t v = static_cast<uint64_t>(value);
        return static_cast<int32_t>(v) ^ static_cast<int32_t>(v >> 32);
    }

	int32_t GetHashCode(const char* value)
	{
		auto hash1 = 5381;
		auto hash2 = hash1;

		int c;

		while ((c = static_cast<unsigned char>(value[0])) != 0) {
			hash1 = (hash1 << 5) + hash1 ^ c;
			c = static_cast<unsigned char>(value[1]);
			if (c == 0)
				break;
			hash2 = (hash2 << 5) + hash2 ^ c;
			value += 2;
		}

		return hash1 + hash2 * 1566083941;
	}

	int32_t GetHashCode(const std::string & value)
	{
		return GetHashCode(value.c_str());
	}

	int32_t CombineHashCodes(const int32_t hash1, const int32_t hash2)
	{
		return (hash1 << 5) + hash1 ^ hash2;
	}
}
