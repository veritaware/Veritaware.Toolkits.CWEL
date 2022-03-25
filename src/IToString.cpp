#include "IToString.hpp"

namespace vwr {
    std::ostream& operator<< (std::ostream& stream, const IToString& iToString)
    {
        stream << iToString.ToString();
        return stream;
    }
}
