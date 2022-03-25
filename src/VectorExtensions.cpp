#include "VectorExtensions.hpp"

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
}
