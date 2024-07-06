#ifndef VWRTK_CWEL_I_COMPARABLE_HPP
#define VWRTK_CWEL_I_COMPARABLE_HPP

#include "Object.hpp"

namespace vwr
{
    class IComparable
    {
    public:
        virtual ~IComparable() = default;
        virtual int CompareTo(const Object& obj) = 0;

        bool operator<(const Object& obj){ return CompareTo(obj) < 0; }
        bool operator>(const Object& obj){ return CompareTo(obj) > 0; }
        bool operator<=(const Object& obj){ return CompareTo(obj) <= 0; }
        bool operator>=(const Object& obj){ return CompareTo(obj) >= 0; }

    protected:
        IComparable() = default;
    };

    template <class T>
    class IComparableT
    {
    public:
        virtual ~IComparableT() = default;
        virtual int CompareTo(const T& other) = 0;

        bool operator<(const T& other){ return CompareTo(other) < 0; }
        bool operator>(const T& other){ return CompareTo(other) > 0; }
        bool operator<=(const T& other){ return CompareTo(other) <= 0; }
        bool operator>=(const T& other){ return CompareTo(other) >= 0; }

    protected:
        IComparableT() = default;
    };
}

#endif //VWRTK_CWEL_I_COMPARABLE_HPP
