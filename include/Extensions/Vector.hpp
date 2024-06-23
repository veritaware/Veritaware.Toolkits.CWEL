#ifndef VWRTK_CWEL_EXTENSIONS_VECTOR_HPP
#define VWRTK_CWEL_EXTENSIONS_VECTOR_HPP

#include <vector>

namespace vwr
{
    template<typename T>
    class vector : public std::vector<T>
    {
    public:
        /// <summary>
        /// Adds the elements of the specified collection to the end of the Vector.
        /// </summary>
        void AddRange(const std::vector<T> &src);

        /// <summary>
        /// Adds the elements of the specified collection to the end of the Vector.
        /// </summary>
        static void AddRange(std::vector<T> &dst, const std::vector<T> &src);

        /// <summary>
        /// Reverses the order of all the elements in the Vector.
        /// </summary>
        void Reverse();

        /// <summary>
        /// Reverses the order of all the elements in the Vector.
        /// </summary>
        static void Reverse(std::vector<T> &v);

        /// <summary>
        /// Removes the first occurrence of a specific object from the Vector.
        /// </summary>
        void Remove(const T &value);

        /// <summary>
        /// Removes the first occurrence of a specific object from the Vector.
        /// </summary>
        static void Remove(std::vector<T> &v, const T &value);
    };
}

#endif //VWRTK_CWEL_EXTENSIONS_VECTOR_HPP
