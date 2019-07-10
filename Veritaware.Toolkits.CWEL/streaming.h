#ifndef VERITAWARE_TOOLKITS_CWEL_STREAMING_
#define VERITAWARE_TOOLKITS_CWEL_STREAMING_
#include <string>

#ifdef VERITAWARETOOLKITSCWEL_EXPORTS
#  define EXPORT __declspec(dllexport)
#else
#  define EXPORT __declspec(dllimport)
#endif

namespace veritaware
{
	struct EXPORT IToString //NOLINT
	{
		virtual ~IToString() = default;
		virtual std::string ToString() const = 0;

		friend std::ostream& operator<< (std::ostream& stream, const IToString& iToString)
		{
			stream << iToString.ToString();
			return stream;
		}

	protected:
		IToString() = default;
	};
}

#endif  //VERITAWARE_TOOLKITS_CWEL_STREAMING_
