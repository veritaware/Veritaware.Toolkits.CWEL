#include "stdafx.h"
#include "vector_ext.h"

template<typename T>
void veritaware::AddRange(std::vector<T> &dst, const std::vector<T> &src)
{
	dst.insert(dst.end(), src.begin(), src.end());
}

template<typename T>
void veritaware::Reverse(std::vector<T> &v)
{
	std::reverse(v.begin(), v.end());
}