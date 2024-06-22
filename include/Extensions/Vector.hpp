#ifndef VWRTK_CWEL_EXTENSIONS_VECTOR_HPP
#define VWRTK_CWEL_EXTENSIONS_VECTOR_HPP

#include <vector>

namespace vwr
{
    /// <summary>
    /// Adds the elements of the specified collection to the end of the Vector.
    /// </summary>
    template<typename T>
    void AddRange(std::vector<T> &dst, const std::vector<T> &src);

    /// <summary>
    /// Reverses the order of all the elements in the Vector.
    /// </summary>
    template<typename T>
    void Reverse(std::vector<T> &v);

    /// <summary>
    /// Removes the first occurrence of a specific object from the Vector.
    /// </summary>
    template<typename T>
    void Remove(std::vector<T> &v, const T &value);
}

#endif //VWRTK_CWEL_EXTENSIONS_VECTOR_HPP
