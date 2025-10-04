#include "Extensions/Vector.hpp"

#include <algorithm>

namespace vwr
{
    template<typename T>
    void vector<T>::AddRange(const std::vector<T> &src)
    {
        AddRange(*this, src);
    }

    template<typename T>
    void vector<T>::AddRange(std::vector<T> &dst, const std::vector<T> &src)
    {
        dst.insert(dst.end(), src.begin(), src.end());
    }

    template<typename T>
    void vector<T>::Reverse()
    {
        Reverse(*this);
    }

    template<typename T>
    void vector<T>::Reverse(std::vector<T> &vec)
    {
        std::reverse(vec.begin(), vec.end());
    }

    template<typename T>
    void vector<T>::Remove(const T &value)
    {
        Remove(*this, value);
    }

    template<typename T>
    void vector<T>::Remove(std::vector<T> &vec, const T &value)
    {
        vec.erase(std::remove(vec.begin(), vec.end(), value), vec.end());
    }
}
