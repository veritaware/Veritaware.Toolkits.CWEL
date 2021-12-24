#ifndef VERITAWARE_TOOLKITS_CWEL_IGETHASHCODE_
#define VERITAWARE_TOOLKITS_CWEL_IGETHASHCODE_
#include <string>

namespace vwr
{
	int32_t GetHashCode(bool value);
	int32_t GetHashCode(int8_t value);
	int32_t GetHashCode(uint8_t value);
	int32_t GetHashCode(int16_t value);
	int32_t GetHashCode(uint16_t value);
	int32_t GetHashCode(int32_t value);
	int32_t GetHashCode(uint32_t value);
	int32_t GetHashCode(int64_t value);
	int32_t GetHashCode(uint64_t value);
    int32_t GetHashCode(size_t value);
	int32_t GetHashCode(float value);
	int32_t GetHashCode(double value);
	int32_t GetHashCode(const char * value);
	int32_t GetHashCode(const std::string & value);
	int32_t CombineHashCodes(int32_t hash1, int32_t hash2);

	class IGetHashCode //NOLINT
	{
	public:
		virtual ~IGetHashCode() = default;
		virtual int32_t GetHashCode() const = 0;

		friend bool operator==(const IGetHashCode& h1, const IGetHashCode& h2)
		{
			return h1.GetHashCode() == h2.GetHashCode();
		}

		friend bool operator!=(const IGetHashCode& h1, const IGetHashCode& h2)
		{
			return h1.GetHashCode() != h2.GetHashCode();
		}

	protected:
		IGetHashCode() = default;
	};
}

#endif  //VERITAWARE_TOOLKITS_CWEL_IGETHASHCODE_