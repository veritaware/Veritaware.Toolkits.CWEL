#include "IGetHashCode.h"

namespace vwr {
	int32_t GetHashCode(const bool value)
	{
		return value;
	}

	int32_t GetHashCode(const int8_t value)
	{
		return static_cast<int32_t>(value) | static_cast<int32_t>(value) << 16;
	}

	int32_t GetHashCode(const uint8_t value)
	{
		return value;
	}

	int32_t GetHashCode(const int16_t value)
	{
		return static_cast<int32_t>(static_cast<uint16_t>(value)) | static_cast<int32_t>(value) << 16;
	}

	int32_t GetHashCode(const uint16_t value)
	{
		return static_cast<int>(value);
	}

	int32_t GetHashCode(const int32_t value)
	{
		return value;
	}

	int32_t GetHashCode(const uint32_t value)
	{
		return static_cast<int>(value);
	}

	int32_t GetHashCode(const int64_t value)
	{
		return static_cast<int32_t>(value) ^ static_cast<int32_t>(value >> 32);
	}

	int32_t GetHashCode(const uint64_t value)
	{
		return static_cast<int32_t>(value) ^ static_cast<int32_t>(value >> 32);
	}

    int32_t GetHashCode(const size_t value)
    {
        return GetHashCode(static_cast<uint64_t>(value));
    }

	int32_t GetHashCode(float value)
	{
		if (value == 0)
			return 0;

		return *reinterpret_cast<int32_t*>(&value);
	}

	int32_t GetHashCode(double value)
	{
		if (value == 0)
			return 0;

		return GetHashCode(*reinterpret_cast<int64_t*>(&value));
	}

	int32_t GetHashCode(const char* value)
	{
		auto hash1 = 5381;
		auto hash2 = hash1;

		int c;

		while ((c = value[0]) != 0) {
			hash1 = (hash1 << 5) + hash1 ^ c;
			c = value[1];
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
