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
    template <typename U, typename = void>
    struct is_hashable : std::false_type {};

    template <typename U>
    struct is_hashable<U, std::void_t<decltype(std::hash<U>{}(std::declval<const U&>()))>> : std::true_type {};

    template <typename U>
    inline constexpr bool is_hashable_v = is_hashable<U>::value;

    template <typename U, typename = void>
    struct is_equality_comparable : std::false_type {};

    template <typename U>
    struct is_equality_comparable<U, std::void_t<
        decltype(std::equal_to<U>{}(std::declval<const U&>(), std::declval<const U&>()))
    >> : std::true_type {};

    template <typename U>
    inline constexpr bool can_use_unordered_set_v = is_hashable_v<U> && is_equality_comparable<U>::value;

    template <typename, typename, typename = void>
    struct is_equality_comparable_with : std::false_type {};

    template <typename Left, typename Right>
    struct is_equality_comparable_with<Left, Right, std::void_t<
        decltype(std::declval<const Left&>() == std::declval<const Right&>())
    >> : std::bool_constant<std::is_convertible_v<
        decltype(std::declval<const Left&>() == std::declval<const Right&>()), bool
    >> {};

    template <typename Left, typename Right>
    inline constexpr bool is_equality_comparable_with_v = is_equality_comparable_with<Left, Right>::value;

    template <typename, typename, typename = void>
    struct is_static_castable : std::false_type {};

    template <typename From, typename To>
    struct is_static_castable<From, To, std::void_t<
        decltype(static_cast<To>(std::declval<From>()))
    >> : std::true_type {};

    template <typename From, typename To>
    inline constexpr bool is_static_castable_v = is_static_castable<From, To>::value;
} // namespace detail

template <typename T>
class query
{
public:
    explicit query(std::vector<T> data) : m_data(std::move(data)) {}
    explicit query(std::initializer_list<T> data) : m_data(data) {}
    template <typename InputIt>
    explicit query(InputIt begin, InputIt end)
    requires (!std::is_integral_v<InputIt>) : m_data(begin, end) {}

    /// Returns a copy of the underlying data vector.
    std::vector<T> get() const { return m_data; }

// region: single element queries
    /// Aggregates the elements of the collection using the specified binary predicate
    /// and the provided seed as the initial accumulator.
    template <typename Predicate>
    T aggregate(T seed, Predicate predicate) const
    {
        T result = seed;
        for(const auto& value : m_data)
            result = std::invoke(predicate, result, value);
        return result;
    }

    /// Aggregates the elements of the collection using the specified binary predicate.
    /// Uses the first element as the initial accumulator and throws if the sequence is empty.
    template <typename Predicate>
    T aggregate(Predicate predicate) const
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
    T& first() &
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence.
    const T& first() const&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence.
    T first() &&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return std::move(m_data.front());
    }

    /// Returns the first element of a sequence.
    T first() const&&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence, or a default value if no element is found.
    T first_or_default() const
    {
        static_assert(
            std::is_default_constructible_v<T>,
            "query<T>::first_or_default() requires T to be default-constructible."
        );
        if(m_data.empty())
            return T{};
        return m_data.front();
    }

    /// Returns the last element of a sequence.
    T& last() &
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence.
    const T& last() const&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence.
    T last() &&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return std::move(m_data.back());
    }

    /// Returns the last element of a sequence.
    T last() const&&
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence, or a default value if no element is found.
    T last_or_default() const
    {
        static_assert(
            std::is_default_constructible_v<T>,
            "query<T>::last_or_default() requires T to be default-constructible."
        );
        if(m_data.empty())
            return T{};
        return m_data.back();
    }

    /// Returns a single, specific element of a sequence.
    T& single() &
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence.
    const T& single() const&
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence.
    T single() &&
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return std::move(m_data.front());
    }

    /// Returns a single, specific element of a sequence.
    T single() const&&
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence, or a default value if that element is not found.
    T single_or_default() const
    {
        static_assert(
            std::is_default_constructible_v<T>,
            "query<T>::single_or_default() requires T to be default-constructible."
        );
        if(m_data.size() > 1)
            throw std::out_of_range("Query contains more than one element.");
        if(m_data.empty())
            return T{};
        return m_data.front();
    }
// endregion: single element queries

// region: query modifiers
    template <typename U>
    query<U> cast() const
    {
        static_assert(
            detail::is_static_castable_v<const T&, U>,
            "query<T>::cast<U>() requires static_cast<U>(const T&) to be well-formed."
        );
        std::vector<U> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
            result.push_back(static_cast<U>(value));
        return query<U>(std::move(result));
    }

    /// Concatenates two sequences.
    query concat(const query& other) const
    {
        std::vector<T> result;
        result.reserve(m_data.size() + other.m_data.size());
        result.insert(result.end(), m_data.begin(), m_data.end());
        result.insert(result.end(), other.m_data.begin(), other.m_data.end());
        return query(std::move(result));
    }

    /// Returns distinct elements from a sequence by using the default equality comparer to compare values.
    query distinct() const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        if constexpr(detail::can_use_unordered_set_v<T>)
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
        return query(std::move(result));
    }

    /// Returns distinct elements from a sequence by using a specified equality comparer to compare values.
    /// The comparer should model an equivalence relation for stable, intuitive results.
    template <typename EqualityComparer>
    query distinct(EqualityComparer comparer) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            bool is_duplicate = false;
            for(const auto& existing : result)
            {
                if(std::invoke(comparer, value, existing))
                {
                    is_duplicate = true;
                    break;
                }
            }
            if(!is_duplicate)
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Produces the set difference of two sequences by using the default equality comparer to compare values.
    /// The result contains only unique elements.
    query except(const query& other) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());

        if constexpr(detail::can_use_unordered_set_v<T>)
        {
            // Use hash-based sets for efficient membership and uniqueness checks when possible.
            std::unordered_set<T> other_set(other.m_data.begin(), other.m_data.end());
            std::unordered_set<T> seen;
            seen.reserve(m_data.size());

            for(const auto& value : m_data)
            {
                if(!other_set.contains(value) && seen.insert(value).second)
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
        return query(std::move(result));
    }

    /// Produces the set difference of two sequences by using the specified equality comparer to compare values.
    /// The result contains only unique elements according to the comparer.
    template <typename EqualityComparer>
    query except(const query& other, EqualityComparer comparer) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            bool is_in_other = false;
            for(const auto& other_value : other.m_data)
            {
                if(std::invoke(comparer, value, other_value))
                {
                    is_in_other = true;
                    break;
                }
            }
            bool is_duplicate = false;
            for(const auto& existing : result)
            {
                if(std::invoke(comparer, value, existing))
                {
                    is_duplicate = true;
                    break;
                }
            }
            if(!is_in_other && !is_duplicate)
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Produces the set intersection of two sequences by using the default equality comparer to compare values.
    /// The result contains only unique elements.
    query intersect(const query& other) const
    {
        std::vector<T> result;
        result.reserve(std::min(m_data.size(), other.m_data.size()));
        if constexpr(detail::can_use_unordered_set_v<T>)
        {
            // Optimized path for hashable types: use hash sets to avoid quadratic scans.
            std::unordered_set<T> other_set;
            other_set.reserve(other.m_data.size());
            other_set.insert(other.m_data.begin(), other.m_data.end());

            std::unordered_set<T> seen;
            seen.reserve(m_data.size());
            for(const auto& value : m_data)
            {
                if(other_set.contains(value))
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
        return query(std::move(result));
    }

    /// Produces the set intersection of two sequences by using the specified equality comparer to compare values.
    /// The result contains only unique elements according to the comparer.
    template <typename EqualityComparer>
    query intersect(const query& other, EqualityComparer comparer) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            bool is_in_other = false;
            for(const auto& other_value : other.m_data)
            {
                if(std::invoke(comparer, value, other_value))
                {
                    is_in_other = true;
                    break;
                }
            }

            bool is_duplicate = false;
            for(const auto& existing : result)
            {
                if(std::invoke(comparer, value, existing))
                {
                    is_duplicate = true;
                    break;
                }
            }

            if(is_in_other && !is_duplicate)
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Correlates the elements of two sequences based on matching keys.
    /// The default equality comparer is used to compare keys selected from each sequence.
    template <typename U, typename OuterKeySelector, typename InnerKeySelector>
    query<std::pair<T, U>> join(const query<U>& other, OuterKeySelector outer_key_selector,
                                InnerKeySelector inner_key_selector) const
    {
        using outer_key_t = std::invoke_result_t<OuterKeySelector&, const T&>;
        using inner_key_t = std::invoke_result_t<InnerKeySelector&, const U&>;
        static_assert(
            detail::is_equality_comparable_with_v<inner_key_t, outer_key_t>,
            "query<T>::join(...) requires selected key types to be comparable with operator==."
        );

        std::vector<std::pair<T, U>> result;
        for(const auto& value : m_data)
        {
            const auto& key = std::invoke(outer_key_selector, value);
            for(const auto& other_value : other.m_data)
            {
                if(std::invoke(inner_key_selector, other_value) == key)
                    result.emplace_back(value, other_value);
            }
        }
        return query<std::pair<T, U>>(std::move(result));
    }

    /// Correlates the elements of two sequences based on matching keys.
    /// The specified equality comparer is used to compare keys selected from each sequence.
    template <typename U, typename OuterKeySelector, typename InnerKeySelector, typename EqualityComparer>
    query<std::pair<T, U>> join(const query<U>& other, OuterKeySelector outer_key_selector,
                                InnerKeySelector inner_key_selector, EqualityComparer comparer) const
    {
        std::vector<std::pair<T, U>> result;
        for(const auto& value : m_data)
        {
            const auto& key = std::invoke(outer_key_selector, value);
            for(const auto& other_value : other.m_data)
            {
                if(std::invoke(comparer, std::invoke(inner_key_selector, other_value), key))
                    result.emplace_back(value, other_value);
            }
        }
        return query<std::pair<T, U>>(std::move(result));
    }

    /// Inverts the order of the elements in a sequence.
    query reverse() const
    {
        std::vector<T> result(m_data.rbegin(), m_data.rend());
        return query(std::move(result));
    }

    /// Determines whether two sequences are equal by comparing the elements by using
    /// the default equality comparer for their type.
    bool sequence_equal(const query& other) const
    {
        if(m_data.size() != other.m_data.size())
            return false;

        return std::equal(m_data.begin(), m_data.end(), other.m_data.begin());
    }

    /// Determines whether two sequences are equal by comparing the elements by using the specified equality comparer.
    template <typename EqualityComparer>
    bool sequence_equal(const query& other, EqualityComparer comparer) const
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
    query skip_while(Predicate predicate) const
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
        return query(std::move(result));
    }

    /// Returns elements from a sequence as long as a specified condition is true,
    /// and then skips the remaining elements.
    template <typename Predicate>
    query take_while(Predicate predicate) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            if(!std::invoke(predicate, value))
                break;
            result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Produces the set union of two sequences.
    /// The result contains only unique elements.
    query unite(const query& other) const
    {
        std::vector<T> result;
        result.reserve(m_data.size() + other.m_data.size());

        if constexpr(detail::can_use_unordered_set_v<T>)
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
        return query(std::move(result));
    }

    /// Filters a sequence of values based on a predicate.
    template <typename Predicate>
    query where(Predicate predicate) const
    {
        std::vector<T> result;
        result.reserve(m_data.size());
        for(const auto& value : m_data)
        {
            if(std::invoke(predicate, value))
                result.push_back(value);
        }
        return query(std::move(result));
    }
// endregion: query modifiers

// region: check queries
    /// Determines whether all elements of a sequence satisfy a condition.
    template <typename Predicate>
    bool all(Predicate predicate) const
    {
        return std::all_of(m_data.begin(), m_data.end(), [&](const T& value) { return std::invoke(predicate, value); });
    }

    /// Determines whether any element of a sequence satisfies a condition.
    template <typename Predicate>
    bool any(Predicate predicate) const
    {
        return std::any_of(m_data.begin(), m_data.end(), [&](const T& value) { return std::invoke(predicate, value); });
    }

    /// Computes the average of a sequence of numeric values.
    /// Only available when T is an arithmetic type.
    /// For integral types, returns a double to avoid truncation.
    template <typename U = T>
    auto average() const -> std::conditional_t<std::is_integral_v<T>, double, T>
    requires (std::is_arithmetic_v<U>) {
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
    bool contains(const T& element) const
    {
        return std::find(m_data.begin(), m_data.end(), element) != m_data.end();
    }

    /// Returns the maximum value in a sequence of values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    T max() const
    requires (std::is_arithmetic_v<U>) {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return *std::max_element(m_data.begin(), m_data.end());
    }

    /// Returns the minimum value in a sequence of values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    T min() const
    requires (std::is_arithmetic_v<U>) {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return *std::min_element(m_data.begin(), m_data.end());
    }

    /// Computes the sum of a sequence of numeric values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    T sum() const
    requires (std::is_arithmetic_v<U>) {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return std::accumulate(m_data.begin(), m_data.end(), static_cast<T>(0));
    }
// endregion: check queries

private:
    std::vector<T> m_data;
};

} // namespace vwr

#endif // CWEL_QUERY_HPP_
