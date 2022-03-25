#ifndef VERITAWARE_TOOLKITS_CWEL_OBJECT_
#define VERITAWARE_TOOLKITS_CWEL_OBJECT_

#include "IGetHashCode.hpp"
#include "IToString.hpp"

namespace vwr
{
    class Object : //NOLINT
        public IGetHashCode, 
        public IToString 
    {
    public:
        Object() = default;
        [[nodiscard]] int32_t GetHashCode() const override;
        [[nodiscard]] std::string ToString() const override;
        [[nodiscard]] const std::type_info& GetType() const;

        [[nodiscard]] bool Equals(const IGetHashCode &obj) const;
        static bool Equals(const IGetHashCode &obj1, const IGetHashCode &obj2);
    };
}

#endif  //VERITAWARE_TOOLKITS_CWEL_OBJECT_
