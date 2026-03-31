/* Property class
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#ifndef CWEL_PROPERTY_HPP_
#define CWEL_PROPERTY_HPP_

#include "event.hpp"

namespace vwr
{

template <typename T>
class property_changed_event_args
{
public:
    property_changed_event_args(const T& old_value, const T& new_value)
        : old_value(old_value), new_value(new_value) {}
    const T& old_value;
    const T& new_value;
};

template <typename T>
class property
{
public:
    property() = default;
    explicit property(const T& value) : property_changed(), m_value(value) {}
    const T& get() const { return m_value; }
    void set(const T& value)
    {
        if (m_value == value)
            return;
        property_changed_event_args<T> event_args(m_value, value);
        m_value = value;
        property_changed(this, event_args);
    }
    const T& operator()() const { return m_value; }
    property& operator=(const T& value)
    {
        set(value);
        return *this;
    }
    property& operator=(const property& other)
    {
        if (this != &other)
            set(other.m_value);
        return *this;
    }

    event_handler<property_changed_event_args<T>> property_changed;
private:
    T m_value;
};

} // namespace vwr

#endif // CWEL_PROPERTY_HPP_
