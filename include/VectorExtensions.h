#ifndef VERITAWARE_TOOLKITS_CWEL_VECTOR_EXTENSIONS_
#define VERITAWARE_TOOLKITS_CWEL_VECTOR_EXTENSIONS_

#include <vector>

namespace vwr
{
	template<typename T>
	void AddRange(std::vector<T> &dst, const std::vector<T> &src);

	template<typename T>
	void Reverse(std::vector<T> &v);
}

#endif //VERITAWARE_TOOLKITS_CWEL_VECTOR_EXTENSIONS_