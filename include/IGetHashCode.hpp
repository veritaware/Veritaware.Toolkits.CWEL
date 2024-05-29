#ifndef VWRTK_CWEL_I_GET_HASHCODE_HPP
#define VWRTK_CWEL_I_GET_HASHCODE_HPP
#include <string>

namespace vwr
{
    int32_t GetHashCode(size_t value);
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

#endif  //VWRTK_CWEL_I_GET_HASHCODE_HPP