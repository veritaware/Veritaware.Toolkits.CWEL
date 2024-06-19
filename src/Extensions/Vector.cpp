#include "Extensions/Vector.hpp"

#include <algorithm>

namespace vwr
{
    template<typename T>
    void AddRange(std::vector<T> &dst, const std::vector<T> &src)
    {
        dst.insert(dst.end(), src.begin(), src.end());
    }

    template<typename T>
    void Reverse(std::vector<T> &v)
    {
        std::reverse(v.begin(), v.end());
    }

    template<typename T>
    void Remove(std::vector<T> &v, const T &value)
    {
        v.erase(std::remove(v.begin(), v.end(), value), v.end());
    }
}
