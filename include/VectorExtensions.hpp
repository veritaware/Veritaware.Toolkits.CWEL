#ifndef VWRTK_CWEL_VECTOR_EXTENSIONS_HPP
#define VWRTK_CWEL_VECTOR_EXTENSIONS_HPP

#include <algorithm>
#include <vector>

namespace vwr
{
	template<typename T>
	void AddRange(std::vector<T> &dst, const std::vector<T> &src);

	template<typename T>
	void Reverse(std::vector<T> &v);
}

#endif //VWRTK_CWEL_VECTOR_EXTENSIONS_HPP