#include "IToString.h"

namespace vwr {
    std::ostream& operator<< (std::ostream& stream, const IToString& iToString)
    {
        stream << iToString.ToString();
        return stream;
    }
}
