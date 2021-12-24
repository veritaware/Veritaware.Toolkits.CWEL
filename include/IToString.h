#ifndef VERITAWARE_TOOLKITS_CWEL_ITOSTREAM_
#define VERITAWARE_TOOLKITS_CWEL_ITOSTREAM_
#include <string>

namespace vwr
{
	class IToString //NOLINT
	{
	public:
		virtual ~IToString() = default;
		virtual std::string ToString() const = 0;

		friend std::ostream& operator<< (std::ostream& stream, const IToString& iToString);

	protected:
		IToString() = default;
	};
}

#endif  //VERITAWARE_TOOLKITS_CWEL_ITOSTREAM_
