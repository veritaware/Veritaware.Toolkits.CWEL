#ifndef VWRTK_CWEL_EXTENSIONS_VECTOR_HPP
#define VWRTK_CWEL_EXTENSIONS_VECTOR_HPP

#include <vector>

namespace vwr
{
    template<typename T>
    void AddRange(std::vector<T> &dst, const std::vector<T> &src);

    template<typename T>
    void Reverse(std::vector<T> &v);

    template<typename T>
    void Remove(std::vector<T> &v, const T &value);
}

#endif //VWRTK_CWEL_EXTENSIONS_VECTOR_HPP
