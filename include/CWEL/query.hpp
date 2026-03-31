/* STL Containers Query extension class
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#ifndef CWEL_QUERY_HPP_
#define CWEL_QUERY_HPP_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace vwr
{

namespace detail
{
// clang-format off
template <typename, typename = void>
struct IsHashable : std::false_type {};

template <typename U>
struct IsHashable<U, std::void_t<decltype(std::hash<U>{}(std::declval<const U&>()))>> : std::true_type {};

template <typename U>
inline constexpr bool IsHashableV = IsHashable<U>::value;

template <typename, typename = void>
struct IsEqualityComparable : std::false_type {};

template <typename U>
struct IsEqualityComparable<U, std::void_t<
    decltype(std::equal_to<U>{}(std::declval<const U&>(), std::declval<const U&>()))
>> : std::true_type {};

template <typename U>
inline constexpr bool CanUseUnorderedSetV = IsHashableV<U> && IsEqualityComparable<U>::value;

template <typename, typename, typename = void>
struct IsEqualityComparableWith : std::false_type {};

template <typename Left, typename Right>
struct IsEqualityComparableWith<Left, Right, std::void_t<
    decltype(std::declval<const Left&>() == std::declval<const Right&>())
>> : std::bool_constant<std::is_convertible_v<
    decltype(std::declval<const Left&>() == std::declval<const Right&>()), bool
>> {};

template <typename Left, typename Right>
inline constexpr bool IsEqualityComparableWithV = IsEqualityComparableWith<Left, Right>::value;

template <typename, typename, typename = void>
struct IsStaticCastable : std::false_type {};

template <typename From, typename To>
struct IsStaticCastable<From, To, std::void_t<
    decltype(static_cast<To>(std::declval<From>()))
>> : std::true_type {};

template <typename From, typename To>
inline constexpr bool IsStaticCastableV = IsStaticCastable<From, To>::value;
//clang-format on
} // namespace detail

template <typename T>
class Query
{
public:
    explicit Query(std::vector<T> data) : m_data(std::move(data)) {}
    explicit Query(std::initializer_list<T> data) : m_data(data) {}
    template <typename InputIt>
    // clang-format off
    explicit Query(InputIt begin, InputIt end)
        requires (!std::is_integral_v<InputIt>) : m_data(begin, end) {}
    // clang-format on

    /// Returns a copy of the underlying data vector.
    std::vector<T> Get() const { return m_data; }

    // region: Single Element Queries
    /// Aggregates the elements of the collection using the specified binary predicate
    /// and the provided seed as the initial accumulator.
    template <typename Predicate>
    T Aggregate(T seed, Predicate predicate) const
    {
        T result = seed;
        for(const auto& value : m_data)
            result = std::invoke(predicate, result, value);
        return result;
    }

    /// Aggregates the elements of the collection using the specified binary predicate.
    /// Uses the first element as the initial accumulator and throws if the sequence is empty.
    template <typename Predicate>
    T Aggregate(Predicate predicate) const
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        auto it = m_data.begin();
        T result = *it;
        ++it;
        for(; it != m_data.end(); ++it)
            result = std::invoke(predicate, result, *it);
        return result;
    }

    /// Returns the first element of a sequence.
    T& First() &
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence.
    const T& First() const&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence.
    T First() &&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return std::move(m_data.front());
    }

    /// Returns the first element of a sequence.
    T First() const&&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence, or a default value if no element is found.
    T FirstOrDefault() const
    {
        //clang-format off
        static_assert(
            std::is_default_constructible_v<T>,
            "Query<T>::FirstOrDefault() requires T to be default-constructible."
        );
        //clang-format on
        if(m_data.empty())
            return T{};
        return m_data.front();
    }

    /// Returns the last element of a sequence.
    T& Last() &
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence.
    const T& Last() const&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence.
    T Last() &&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return std::move(m_data.back());
    }

    /// Returns the last element of a sequence.
    T Last() const&&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence, or a default value if no element is found.
    T LastOrDefault() const
    {
        //clang-format off
        static_assert(
            std::is_default_constructible_v<T>,
            "Query<T>::LastOrDefault() requires T to be default-constructible."
        );
        //clang-format on
        if(m_data.empty())
            return T{};
        return m_data.back();
    }

    /// Returns a single, specific element of a sequence.
    T& Single() &
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence.
    const T& Single() const&
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence.
    T Single() &&
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return std::move(m_data.front());
    }

    /// Returns a single, specific element of a sequence.
    T Single() const&&
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence, or a default value if that element is not found.
    T SingleOrDefault() const
    {
        //clang-format off
        static_assert(
            std::is_default_constructible_v<T>,
            "Query<T>::SingleOrDefault() requires T to be default-constructible."
        );
        //clang-format on
        if(m_data.size() > 1)
            throw std::out_of_range("Query contains more than one element.");
        if(m_data.empty())
            return T{};
        return m_data.front();
    }
    // endregion: Single Element Queries

    // region: Query Modifiers
    template <typename U>
    Query<U> Cast() const
    {
        static_assert(
            detail::IsStaticCastableV<const T&, U>,
            "Query<T>::Cast<U>() requires static_cast<U>(const T&) to be well-formed."
        );
        std::vector<U> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
            result.push_back(static_cast<U>(value));
        return Query<U>(std::move(result));
    }

    /// Concatenates two sequences.
    Query Concat(const Query& other) const
    {
        std::vector<T> result;
        result.reserve(m_data.size() + other.m_data.size());
        result.insert(result.end(), m_data.begin(), m_data.end());
        result.insert(result.end(), other.m_data.begin(), other.m_data.end());
        return Query(std::move(result));
    }

    /// Returns distinct elements from a sequence by using the default equality comparer to compare values.
    Query Distinct() const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        if constexpr(detail::CanUseUnorderedSetV<T>)
        {
            std::unordered_set<T> seen;
            seen.reserve(m_data.size());

            for(const auto& value : m_data)
            {
                if(seen.insert(value).second)
                    result.push_back(value);
            }
        }
        else
        {
            for(const auto& value : m_data)
            {
                if(std::find(result.begin(), result.end(), value) == result.end())
                    result.push_back(value);
            }
        }
        return Query(std::move(result));
    }

    /// Returns distinct elements from a sequence by using a specified equality comparer to compare values.
    /// The comparer should model an equivalence relation for stable, intuitive results.
    template <typename EqualityComparer>
    Query Distinct(EqualityComparer comparer) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            bool isDuplicate = false;
            for(const auto& existing : result)
            {
                if(std::invoke(comparer, value, existing))
                {
                    isDuplicate = true;
                    break;
                }
            }
            if(!isDuplicate)
                result.push_back(value);
        }
        return Query(std::move(result));
    }

    /// Produces the set difference of two sequences by using the default equality comparer to compare values.
    /// The result contains only unique elements.
    Query Except(const Query& other) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());

        if constexpr(detail::CanUseUnorderedSetV<T>)
        {
            // Use hash-based sets for efficient membership and uniqueness checks when possible.
            std::unordered_set<T> otherSet(other.m_data.begin(), other.m_data.end());
            std::unordered_set<T> seen;
            seen.reserve(m_data.size());

            for(const auto& value : m_data)
            {
                if(!otherSet.contains(value) && seen.insert(value).second)
                {
                    result.push_back(value);
                }
            }
        }
        else
        {
            // Fallback to linear searches when T cannot be used in an unordered_set.
            for(const auto& value : m_data)
            {
                if(std::find(other.m_data.begin(), other.m_data.end(), value) == other.m_data.end() &&
                   std::find(result.begin(), result.end(), value) == result.end())
                {
                    result.push_back(value);
                }
            }
        }
        return Query(std::move(result));
    }

    /// Produces the set difference of two sequences by using the specified equality comparer to compare values.
    /// The result contains only unique elements according to the comparer.
    template <typename EqualityComparer>
    Query Except(const Query& other, EqualityComparer comparer) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            bool isInOther = false;
            for(const auto& otherValue : other.m_data)
            {
                if(std::invoke(comparer, value, otherValue))
                {
                    isInOther = true;
                    break;
                }
            }
            bool isDuplicate = false;
            for(const auto& existing : result)
            {
                if(std::invoke(comparer, value, existing))
                {
                    isDuplicate = true;
                    break;
                }
            }
            if(!isInOther && !isDuplicate)
                result.push_back(value);
        }
        return Query(std::move(result));
    }

    /// Produces the set intersection of two sequences by using the default equality comparer to compare values.
    /// The result contains only unique elements.
    Query Intersect(const Query& other) const
    {
        std::vector<T> result;
        result.reserve(std::min(m_data.size(), other.m_data.size()));
        if constexpr(detail::CanUseUnorderedSetV<T>)
        {
            // Optimized path for hashable types: use hash sets to avoid quadratic scans.
            std::unordered_set<T> otherSet;
            otherSet.reserve(other.m_data.size());
            otherSet.insert(other.m_data.begin(), other.m_data.end());

            std::unordered_set<T> seen;
            seen.reserve(m_data.size());
            for(const auto& value : m_data)
            {
                if(otherSet.contains(value))
                {
                    // seen.insert(value).second is true only if value was not already present.
                    if(seen.insert(value).second)
                        result.push_back(value);
                }
            }
        }
        else
        {
            // Fallback for non-hashable types: preserve original std::find-based behavior.
            for(const auto& value : m_data)
            {
                if(std::find(other.m_data.begin(), other.m_data.end(), value) != other.m_data.end() &&
                   std::find(result.begin(), result.end(), value) == result.end())
                    result.push_back(value);
            }
        }
        return Query(std::move(result));
    }

    /// Produces the set intersection of two sequences by using the specified equality comparer to compare values.
    /// The result contains only unique elements according to the comparer.
    template <typename EqualityComparer>
    Query Intersect(const Query& other, EqualityComparer comparer) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            bool isInOther = false;
            for(const auto& otherValue : other.m_data)
            {
                if(std::invoke(comparer, value, otherValue))
                {
                    isInOther = true;
                    break;
                }
            }

            bool isDuplicate = false;
            for(const auto& existing : result)
            {
                if(std::invoke(comparer, value, existing))
                {
                    isDuplicate = true;
                    break;
                }
            }

            if(isInOther && !isDuplicate)
                result.push_back(value);
        }
        return Query(std::move(result));
    }

    //clang-format off
    /// Correlates the elements of two sequences based on matching keys.
    /// The default equality comparer is used to compare keys selected from each sequence.
    template <typename U, typename OuterKeySelector, typename InnerKeySelector>
    Query<std::pair<T, U>> Join(const Query<U>& other, OuterKeySelector outerKeySelector,
                                InnerKeySelector innerKeySelector) const
    //clang-format on
    {
        using outerKeyT = std::invoke_result_t<OuterKeySelector&, const T&>;
        using innerKeyT = std::invoke_result_t<InnerKeySelector&, const U&>;
        static_assert(
            detail::IsEqualityComparableWithV<innerKeyT, outerKeyT>,
            "Query<T>::Join(...) requires selected key types to be comparable with operator==."
        );

        std::vector<std::pair<T, U>> result;
        for(const auto& value : m_data)
        {
            const auto& key = std::invoke(outerKeySelector, value);
            for(const auto& otherValue : other.m_data)
            {
                if(std::invoke(innerKeySelector, otherValue) == key)
                    result.emplace_back(value, otherValue);
            }
        }
        return Query<std::pair<T, U>>(std::move(result));
    }

    //clang-format off
    /// Correlates the elements of two sequences based on matching keys.
    /// The specified equality comparer is used to compare keys selected from each sequence.
    template <typename U, typename OuterKeySelector, typename InnerKeySelector, typename EqualityComparer>
    Query<std::pair<T, U>> Join(const Query<U>& other, OuterKeySelector outerKeySelector,
                                InnerKeySelector innerKeySelector, EqualityComparer comparer) const
    //clang-format on
    {
        std::vector<std::pair<T, U>> result;
        for(const auto& value : m_data)
        {
            const auto& key = std::invoke(outerKeySelector, value);
            for(const auto& otherValue : other.m_data)
            {
                if(std::invoke(comparer, std::invoke(innerKeySelector, otherValue), key))
                    result.emplace_back(value, otherValue);
            }
        }
        return Query<std::pair<T, U>>(std::move(result));
    }

    /// Inverts the order of the elements in a sequence.
    Query Reverse() const
    {
        std::vector<T> result(m_data.rbegin(), m_data.rend());
        return Query(std::move(result));
    }

    /// Determines whether two sequences are equal by comparing the elements by using
    /// the default equality comparer for their type.
    bool SequenceEqual(const Query& other) const
    {
        if(m_data.size() != other.m_data.size())
            return false;

        return std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
    }

    /// Determines whether two sequences are equal by comparing the elements by using the specified equality comparer.
    template <typename EqualityComparer>
    bool SequenceEqual(const Query& other, EqualityComparer comparer) const
    {
        if(m_data.size() != other.m_data.size())
            return false;
        for(size_t i = 0; i < m_data.size(); ++i)
        {
            if(!std::invoke(comparer, m_data[i], other.m_data[i]))
                return false;
        }
        return true;
    }

    /// Bypasses elements in a sequence as long as a specified condition is true,
    /// and then returns the remaining elements.
    template <typename Predicate>
    Query SkipWhile(Predicate predicate) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        bool skipping = true;
        for(const auto& value : m_data)
        {
            if(skipping && std::invoke(predicate, value))
                continue;
            skipping = false;
            result.push_back(value);
        }
        return Query(std::move(result));
    }

    /// Returns elements from a sequence as long as a specified condition is true,
    /// and then skips the remaining elements.
    template <typename Predicate>
    Query TakeWhile(Predicate predicate) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            if(!std::invoke(predicate, value))
                break;
            result.push_back(value);
        }
        return Query(std::move(result));
    }

    /// Produces the set union of two sequences.
    /// The result contains only unique elements.
    Query Unite(const Query& other) const
    {
        std::vector<T> result;
        result.reserve(m_data.size() + other.m_data.size());

        if constexpr(detail::CanUseUnorderedSetV<T>)
        {
            std::unordered_set<T> seen;
            seen.reserve(m_data.size() + other.m_data.size());

            for(const auto& value : m_data)
            {
                if(seen.insert(value).second)
                    result.push_back(value);
            }

            for(const auto& value : other.m_data)
            {
                if(seen.insert(value).second)
                    result.push_back(value);
            }
        }
        else
        {
            for(const auto& value : m_data)
            {
                if(std::find(result.begin(), result.end(), value) == result.end())
                    result.push_back(value);
            }

            for(const auto& value : other.m_data)
            {
                if(std::find(result.begin(), result.end(), value) == result.end())
                    result.push_back(value);
            }
        }
        return Query(std::move(result));
    }

    /// Filters a sequence of values based on a predicate.
    template <typename Predicate>
    Query Where(Predicate predicate) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            if(std::invoke(predicate, value))
                result.push_back(value);
        }
        return Query(std::move(result));
    }
    // endregion: Query Modifiers

    // region: Check Queries
    /// Determines whether all elements of a sequence satisfy a condition.
    template <typename Predicate>
    bool All(Predicate predicate) const
    {
        return std::all_of(m_data.begin(), m_data.end(), [&](const T& value) { return std::invoke(predicate, value); });
    }

    /// Determines whether any element of a sequence satisfies a condition.
    template <typename Predicate>
    bool Any(Predicate predicate) const
    {
        return std::any_of(m_data.begin(), m_data.end(), [&](const T& value) { return std::invoke(predicate, value); });
    }

    /// Computes the average of a sequence of numeric values.
    /// Only available when T is an arithmetic type.
    /// For integral types, returns a double to avoid truncation.
    template <typename U = T>
    auto Average() const -> std::conditional_t<std::is_integral_v<T>, double, T>
        requires(std::is_arithmetic_v<U>)
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        using Result = std::conditional_t<std::is_integral_v<T>, double, T>;
        using Accumulator = std::conditional_t<std::is_integral_v<T>, double, long double>;
        const Accumulator total = std::accumulate(
            m_data.begin(), m_data.end(), Accumulator{0},
            [](Accumulator acc, const T& value) { return acc + static_cast<Accumulator>(value); }
        );
        return static_cast<Result>(total / static_cast<Accumulator>(m_data.size()));
    }

    /// Determines whether a sequence contains a specified element by using the default equality comparer.
    bool Contains(const T& element) const { return std::find(m_data.begin(), m_data.end(), element) != m_data.end(); }

    /// Returns the maximum value in a sequence of values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    T Max() const
        requires(std::is_arithmetic_v<U>)
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return *std::max_element(m_data.begin(), m_data.end());
    }

    /// Returns the minimum value in a sequence of values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    T Min() const
        requires(std::is_arithmetic_v<U>)
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return *std::min_element(m_data.begin(), m_data.end());
    }

    /// Computes the sum of a sequence of numeric values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    T Sum() const
        requires(std::is_arithmetic_v<U>)
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return std::accumulate(m_data.begin(), m_data.end(), static_cast<T>(0));
    }
    // endregion: Check Queries

#ifdef USE_STL_NAMING
    // clang-format off
    std::vector<T> get() const { return Get(); }
    T aggregate(T seed, auto predicate) const { return Aggregate(seed, predicate); }
    T aggregate(auto predicate) const { return Aggregate(predicate); }
    T& first() & { return First(); }
    const T& first() const& { return First(); }
    T first() && { return First(); }
    T first() const&& { return First(); }
    T first_or_default() const { return FirstOrDefault(); }
    T last() & { return Last(); }
    const T& last() const& { return Last(); }
    T last() && { return Last(); }
    T last() const&& { return Last(); }
    T last_or_default() const { return LastOrDefault(); }
    T& single() & { return Single(); }
    const T& single() const& { return Single(); }
    T single() && { return Single(); }
    T single() const&& { return Single(); }
    T single_or_default() const { return SingleOrDefault(); }
    template <typename U> Query<U> cast() const { return Cast<U>(); }
    Query concat(const Query& other) const { return Concat(other); }
    Query distinct() const { return Distinct(); }
    template <typename EqualityComparer> Query distinct(EqualityComparer comparer) const { return Distinct(comparer); }
    Query except(const Query& other) const { return Except(other); }
    template <typename EqualityComparer> Query except(const Query& other, EqualityComparer comparer) const { return Except(other, comparer); }
    Query intersect(const Query& other) const { return Intersect(other); }
    template <typename EqualityComparer> Query intersect(const Query& other, EqualityComparer comparer) const { return Intersect(other, comparer); }
    template <typename U, typename OuterKeySelector, typename InnerKeySelector> Query<std::pair<T, U>> join(const Query<U>& other, OuterKeySelector outerKeySelector, InnerKeySelector innerKeySelector) const { return Join(other, outerKeySelector, innerKeySelector); }
    template <typename U, typename OuterKeySelector, typename InnerKeySelector, typename EqualityComparer> Query<std::pair<T, U>> join(const Query<U>& other, OuterKeySelector outerKeySelector, InnerKeySelector innerKeySelector, EqualityComparer comparer) const { return Join(other, outerKeySelector, innerKeySelector, comparer); }
    Query reverse() const { return Reverse(); }
    bool sequence_equal(const Query& other) const { return SequenceEqual(other); }
    template <typename EqualityComparer> bool sequence_equal(const Query& other, EqualityComparer comparer) const { return SequenceEqual(other, comparer); }
    template <typename Predicate> Query skip_while(Predicate predicate) const { return SkipWhile(predicate); }
    template <typename Predicate> Query take_while(Predicate predicate) const { return TakeWhile(predicate); }
    Query unite(const Query& other) const { return Unite(other); }
    template <typename Predicate> Query where(Predicate predicate) const { return Where(predicate); }
    template <typename Predicate> bool all(Predicate predicate) const { return All(predicate); }
    template <typename Predicate> bool any(Predicate predicate) const { return Any(predicate); }
    template <typename U = T> auto average() const { return Average<U>(); }
    bool contains(const T& element) const { return Contains(element); }
    template <typename U = T> T max() const { return Max<U>(); }
    template <typename U = T> T min() const { return Min<U>(); }
    template <typename U = T> T sum() const { return Sum<U>(); }
// clang-format on
#endif // USE_STL_NAMING

private:
    std::vector<T> m_data;
};

#ifdef USE_STL_NAMING
// clang-format off
template <typename T>
using query = Query<T>;
// clang-format on
#endif // USE_STL_NAMING

} // namespace vwr

#endif // CWEL_QUERY_HPP_
