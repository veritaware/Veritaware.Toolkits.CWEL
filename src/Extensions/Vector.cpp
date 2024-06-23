#include "Extensions/Vector.hpp"

#include <algorithm>

namespace vwr
{
    template<typename T>
    void vwr::vector<T>::AddRange(const std::vector<T> &src)
    {
        AddRange(*this, src);
    }

    template<typename T>
    void vwr::vector<T>::AddRange(std::vector<T> &dst, const std::vector<T> &src)
    {
        dst.insert(dst.end(), src.begin(), src.end());
    }

    template<typename T>
    void vwr::vector<T>::Reverse()
    {
        Reverse(*this);
    }

    template<typename T>
    void vwr::vector<T>::Reverse(std::vector<T> &v)
    {
        std::reverse(v.begin(), v.end());
    }

    template<typename T>
    void vwr::vector<T>::Remove(const T &value)
    {
        Remove(*this, value);
    }

    template<typename T>
    void vwr::vector<T>::Remove(std::vector<T> &v, const T &value)
    {
        v.erase(std::remove(v.begin(), v.end(), value), v.end());
    }
}
