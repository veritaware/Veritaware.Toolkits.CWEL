#include "Object.h"

int32_t vwr::Object::GetHashCode() {
    return 0;
}

std::string vwr::Object::ToString() {
    return "vwr::Object";
}

std::string vwr::Object::GetType() {
    return "vwr::Object";
}

bool vwr::Object::Equals(const vwr::IGetHashCode &obj) const {
    return Equals(*this, obj);
}

bool vwr::Object::Equals(const vwr::IGetHashCode &obj1, const vwr::IGetHashCode &obj2) {
    return obj1.GetHashCode() == obj2.GetHashCode();
}
