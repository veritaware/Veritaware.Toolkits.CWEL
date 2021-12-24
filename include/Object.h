#ifndef VERITAWARE_TOOLKITS_CWEL_OBJECT_
#define VERITAWARE_TOOLKITS_CWEL_OBJECT_

#include "IGetHashCode.h"
#include "IToString.h"

namespace vwr
{
    class Object : //NOLINT
        public IGetHashCode, 
        public IToString 
    {
    public:
        Object() = default;
        int32_t GetHashCode() const override;
        std::string ToString() const override;
        const std::type_info& GetType() const;

        bool Equals(const IGetHashCode &obj) const;
        static bool Equals(const IGetHashCode &obj1, const IGetHashCode &obj2);
    };
}

#endif  //VERITAWARE_TOOLKITS_CWEL_OBJECT_
