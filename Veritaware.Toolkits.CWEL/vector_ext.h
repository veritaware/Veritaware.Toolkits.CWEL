#ifndef VERITAWARE_TOOLKITS_CWEL_VECTOR_EXT_
#define VERITAWARE_TOOLKITS_CWEL_VECTOR_EXT_

#include <vector>

namespace veritaware
{
	template<typename T>
	void AddRange(std::vector<T> &dst, const std::vector<T> &src);

	template<typename T>
	void Reverse(std::vector<T> &v);
}

#endif //VERITAWARE_TOOLKITS_CWEL_VECTOR_EXT_