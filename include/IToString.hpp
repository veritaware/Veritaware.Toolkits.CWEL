#ifndef VWRTK_CWEL_I_TO_STRING_HPP
#define VWRTK_CWEL_I_TO_STRING_HPP

#include <sstream>
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

#endif  //VWRTK_CWEL_I_TO_STRING_HPP
