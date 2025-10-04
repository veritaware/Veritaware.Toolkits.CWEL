#include "IGetHashCode.hpp"

namespace vwr {
    static constexpr uint64_t IntShift = 32;
    static constexpr int32_t Magic1 = 5381;
    static constexpr int32_t Magic2 = 1566083941;
    static constexpr int32_t MagicShift = 5;

    int32_t GetHashCode(const size_t value)
    {
        uint64_t const v = value; // NOLINT(*-identifier-length)
        return static_cast<int32_t>(v) ^ static_cast<int32_t>(v >> IntShift);
    }

    int32_t GetHashCode(const char* value)
    {
        auto hash1 = Magic1;
        auto hash2 = Magic1;

        int temp;

        while ((temp = static_cast<unsigned char>(value[0])) != 0) {
            hash1 = (hash1 << MagicShift) + hash1 ^ temp;
            temp = static_cast<unsigned char>(value[1]);
            if (temp == 0)
            {
                break;
            }
            hash2 = (hash2 << MagicShift) + hash2 ^ temp;
            value += 2;
        }

        // ReSharper disable once CppRedundantParentheses
        return hash1 + (hash2 * Magic2);
    }

    int32_t GetHashCode(const std::string & value)
    {
        return GetHashCode(value.c_str());
    }

    int32_t CombineHashCodes(const int32_t hash1, const int32_t hash2)
    {
        // ReSharper disable once CppRedundantParentheses
        return ((hash1 << MagicShift) + hash1) ^ hash2;
    }
}
