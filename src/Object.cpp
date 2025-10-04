#include "Object.hpp"

#if defined(__GNUC__) || defined(__GNUG__)
#include <cxxabi.h>
#endif

int32_t vwr::Object::GetHashCode() const
{
    return vwr::GetHashCode(reinterpret_cast<int64_t>(this));
}

std::string vwr::Object::ToString() const
{
    return GetTypeName();
}

const std::type_info& vwr::Object::GetType() const
{
    return typeid(*this);
}

std::string vwr::Object::GetTypeName() const
{
#if defined(__GNUC__) || defined(__GNUG__)
    return
    {
        abi::__cxa_demangle(
            GetType().name(),
            nullptr,
            nullptr,
            nullptr
    )};
#else
    return GetType().name();
#endif
}

bool vwr::Object::Equals(const IGetHashCode &obj) const
{
    return Equals(*this, obj);
}

bool vwr::Object::Equals(const IGetHashCode &obj1, const IGetHashCode &obj2)
{
    return obj1.GetHashCode() == obj2.GetHashCode();
}
