#include "Object.h"

int32_t vwr::Object::GetHashCode() const {
    return vwr::GetHashCode((int64_t)this);
}

std::string vwr::Object::ToString() const {
    return "vwr::Object";
}

std::string vwr::Object::GetType() const {
    return "vwr::Object";
}

bool vwr::Object::Equals(const vwr::IGetHashCode &obj) const {
    return Equals(*this, obj);
}

bool vwr::Object::Equals(const vwr::IGetHashCode &obj1, const vwr::IGetHashCode &obj2) {
    return obj1.GetHashCode() == obj2.GetHashCode();
}
