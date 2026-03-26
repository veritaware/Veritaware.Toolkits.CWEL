/* STL Containers Query extension class
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#ifndef CWEL_QUERY_HPP_
#define CWEL_QUERY_HPP_

#include <initializer_list>
#include <numeric>
#include <stdexcept>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace vwr
{

template <typename T>
class query
{
public:
    explicit query(std::vector<T> data) : m_data(std::move(data)) {}
    explicit query(std::initializer_list<T> data) : m_data(data) {}
    template <typename InputIt>
    explicit query(InputIt begin, InputIt end) : m_data(begin, end) {}

    /// Returns a copy of the underlying data vector.
    std::vector<T> get() const { return m_data; }

#pragma region single element queries
    /// Aggregates the elements of the collection using the specified binary predicate.
    template <typename Predicate>
    T aggregate(Predicate&& predicate) const
    {
        T result{};
        for(const auto& value : m_data)
            result = predicate(result, value);
        return result;
    }

    /// Returns the first element of a sequence.
    T& first()
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence.
    const T& first() const
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.front();
    }

    /// Returns the first element of a sequence, or a default value if no element is found.
    T first_or_default() const
    {
        if(m_data.empty())
            return T{};
        return m_data.front();
    }

    /// Returns the last element of a sequence.
    T& last()
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence.
    const T& last() const
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return m_data.back();
    }

    /// Returns the last element of a sequence, or a default value if no element is found.
    T last_or_default() const
    {
        if(m_data.empty())
            return T{};
        return m_data.back();
    }

    /// Returns a single, specific element of a sequence.
    T& single()
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence.
    const T& single() const
    {
        if(m_data.size() != 1)
            throw std::out_of_range("Query does not contain exactly one element.");
        return m_data.front();
    }

    /// Returns a single, specific element of a sequence, or a default value if that element is not found.
    T single_or_default() const
    {
        if(m_data.size() > 1)
            throw std::out_of_range("Query contains more than one element.");
        if(m_data.empty())
            return T{};
        return m_data.front();
    }
#pragma endregion single element queries

#pragma region query modifiers
    template <typename U>
    query<U> cast() const
    {
        std::vector<U> result;
        for(const auto& value : m_data)
        {
            if constexpr(std::is_convertible_v<T, U>)
                result.push_back(static_cast<U>(value));
            else
                throw std::bad_cast();
        }
        return query<U>(std::move(result));
    }

    /// Concatenates two sequences.
    query concat(const query& other) const
    {
        std::vector<T> result = m_data;
        result.insert(result.end(), other.m_data.begin(), other.m_data.end());
        return query(std::move(result));
    }

    /// Returns distinct elements from a sequence by using the default equality comparer to compare values.
    query distinct() const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            if(std::find(result.begin(), result.end(), value) == result.end())
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Returns distinct elements from a sequence by using a specified equality comparer to compare values.
    template <typename EqualityComparer>
    query distinct(EqualityComparer&& comparer) const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            bool is_duplicate = false;
            for(const auto& existing : result)
            {
                if(comparer(value, existing))
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
    query except(const query& other) const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            if(std::find(other.m_data.begin(), other.m_data.end(), value) == other.m_data.end())
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Produces the set difference of two sequences by using the specified equality comparer to compare values.
    template <typename EqualityComparer>
    query except(const query& other, EqualityComparer&& comparer) const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            bool is_in_other = false;
            for(const auto& other_value : other.m_data)
            {
                if(comparer(value, other_value))
                {
                    is_in_other = true;
                    break;
                }
            }
            if(!is_in_other)
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Produces the set intersection of two sequences by using the default equality comparer to compare values.
    query intersect(const query& other) const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            if(std::find(other.m_data.begin(), other.m_data.end(), value) != other.m_data.end())
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Produces the set intersection of two sequences by using the specified equality comparer to compare values.
    template <typename EqualityComparer>
    query intersect(const query& other, EqualityComparer&& comparer) const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            for(const auto& other_value : other.m_data)
            {
                if(comparer(value, other_value))
                {
                    result.push_back(value);
                    break;
                }
            }
        }
        return query(std::move(result));
    }

    /// Correlates the elements of two sequences based on matching keys.
    /// The default equality comparer is used to compare keys.
    template <typename U, typename KeySelector>
    query<std::pair<T, U>> join(const query<U>& other, KeySelector&& key_selector) const
    {
        std::vector<std::pair<T, U>> result;
        for(const auto& value : m_data)
        {
            const auto key = key_selector(value);
            for(const auto& other_value : other.m_data)
            {
                if(key_selector(other_value) == key)
                    result.emplace_back(value, other_value);
            }
        }
        return query<std::pair<T, U>>(std::move(result));
    }

    /// Correlates the elements of two sequences based on matching keys.
    /// The specified equality comparer is used to compare keys.
    template <typename U, typename KeySelector, typename EqualityComparer>
    query<std::pair<T, U>> join(const query<U>& other, KeySelector&& key_selector, EqualityComparer&& comparer) const
    {
        std::vector<std::pair<T, U>> result;
        for(const auto& value : m_data)
        {
            const auto key = key_selector(value);
            for(const auto& other_value : other.m_data)
            {
                if(comparer(key_selector(other_value), key))
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
        for(size_t i = 0; i < m_data.size(); ++i)
        {
            if(m_data[i] != other.m_data[i])
                return false;
        }
        return true;
    }

    /// Determines whether two sequences are equal by comparing the elements by using the specified equality comparer.
    template <typename EqualityComparer>
    bool sequence_equal(const query& other, EqualityComparer&& comparer) const
    {
        if(m_data.size() != other.m_data.size())
            return false;
        for(size_t i = 0; i < m_data.size(); ++i)
        {
            if(!comparer(m_data[i], other.m_data[i]))
                return false;
        }
        return true;
    }

    /// Bypasses elements in a sequence as long as a specified condition is true,
    /// and then returns the remaining elements.
    template <typename Predicate>
    query skip_while(Predicate&& predicate) const
    {
        std::vector<T> result;
        bool           skipping = true;
        for(const auto& value : m_data)
        {
            if(skipping && predicate(value))
                continue;
            skipping = false;
            result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Returns elements from a sequence as long as a specified condition is true,
    /// and then skips the remaining elements.
    template <typename Predicate>
    query take_while(Predicate&& predicate) const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            if(!predicate(value))
                break;
            result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Produces the set union of two sequences.
    query unite(const query& other) const
    {
        std::vector<T> result = m_data;
        for(const auto& value : other.m_data)
        {
            if(std::find(result.begin(), result.end(), value) == result.end())
                result.push_back(value);
        }
        return query(std::move(result));
    }

    /// Filters a sequence of values based on a predicate.
    template <typename Predicate>
    query where(Predicate&& predicate) const
    {
        std::vector<T> result;
        for(const auto& value : m_data)
        {
            if(predicate(value))
                result.push_back(value);
        }
        return query(std::move(result));
    }
#pragma endregion query modifiers

#pragma region check queries
    /// Determines whether all elements of a sequence satisfy a condition.
    template <typename Predicate>
    bool all(Predicate&& predicate) const
    {
        for(const auto& value : m_data)
        {
            if(!predicate(value))
                return false;
        }
        return true;
    }

    /// Determines whether any element of a sequence satisfies a condition.
    template <typename Predicate>
    bool any(Predicate&& predicate) const
    {
        for(const auto& value : m_data)
        {
            if(predicate(value))
                return true;
        }
        return false;
    }

    /// Computes the average of a sequence of numeric values.
    /// Only available when T is an arithmetic type.
    /// For integral types, returns a double to avoid truncation.
    template <typename U = T>
    std::enable_if_t<std::is_arithmetic_v<U>, double> average() const
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        const double sum = std::accumulate(m_data.begin(), m_data.end(), 0.0);
        return sum / static_cast<double>(m_data.size());
    }

    /// Determines whether a sequence contains a specified element by using the default equality comparer.
    bool contains(const T& element) const
    {
        for(const auto& value : m_data)
        {
            if(value == element)
                return true;
        }
        return false;
    }

    /// Returns the maximum value in a sequence of values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    std::enable_if_t<std::is_arithmetic_v<U>, T> max() const
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return *std::max_element(m_data.begin(), m_data.end());
    }

    /// Returns the minimum value in a sequence of values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    std::enable_if_t<std::is_arithmetic_v<U>, T> min() const
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return *std::min_element(m_data.begin(), m_data.end());
    }

    /// Computes the sum of a sequence of numeric values.
    /// Only available when T is an arithmetic type.
    template <typename U = T>
    std::enable_if_t<std::is_arithmetic_v<U>, double> sum() const
    {
        if(m_data.empty())
            throw std::out_of_range("Query is empty.");
        return std::accumulate(m_data.begin(), m_data.end(), 0.0);
    }
#pragma endregion check queries

private:
    std::vector<T> m_data;
};

} // namespace vwr

#endif // CWEL_QUERY_HPP_
