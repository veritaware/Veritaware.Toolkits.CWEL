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
class PropertyChangedEventArgs
{
public:
    //clang-format off
    PropertyChangedEventArgs(const T& oldValue, const T& newValue) // NOLINT(*-easily-swappable-parameters)
        : OldValue(oldValue), NewValue(newValue) {}
    //clang-format on
    const T OldValue;
    const T NewValue;

#ifdef USE_STL_NAMING
    // clang-format off
    const T& old_value() const { return OldValue; }
    const T& new_value() const { return NewValue; }
// clang-format on
#endif // USE_STL_NAMING
};

template <typename T>
class Property
{
public:
    Property() : PropertyChanged(), m_value(), m_isInitialized(false) {}
    explicit Property(const T& value) : PropertyChanged(), m_value(value), m_isInitialized(true) {}

    // clang-format off
    /// Copies the value from another Property without copying the event handlers, as those are not copyable or movable.
    explicit Property(const Property& other)
        : PropertyChanged(), m_value(other.m_value), m_isInitialized(other.m_isInitialized) {}
    // clang-format on

    /// Copies the value from another Property without copying the event handlers, as those are not copyable or movable,
    /// and raises the PropertyChanged event if the value has changed.
    Property& operator=(const Property& other)
    {
        if(this != &other)
            Set(other.m_value);
        return *this;
    }

    /// Disabled due to event handlers not being movable.
    explicit Property(Property&& other) = delete;
    /// Disabled due to event handlers not being movable.
    Property& operator=(Property&& other) = delete;

    /// Returns the Property value.
    const T& Get() const { return m_value; }

    /// Returns the Property value.
    const T& operator()() const { return m_value; }

    /// Sets the Property value and raises the PropertyChanged event if the value has changed.
    /// The PropertyChanged event is not raised if the value is being initialized for the first time.
    void Set(const T& value)
    {
        if(!m_isInitialized)
        {
            m_value = value;
            m_isInitialized = true;
            return;
        }
        if(m_value == value)
            return;
        PropertyChangedEventArgs<T> eventArgs(m_value, value);
        m_value = value;
        PropertyChanged(this, eventArgs);
    }

    /// Sets the Property value and raises the PropertyChanged event if the value has changed.
    /// The PropertyChanged event is not raised if the value is being initialized for the first time.
    Property& operator=(const T& value)
    {
        Set(value);
        return *this;
    }

    /// Event called when the Property value has changed from its previous state.
    /// The Property value must NOT be modified in the PropertyChanged callback, as it will cause a deadlock.
    EventHandler<PropertyChangedEventArgs<T>> PropertyChanged;

#ifdef USE_STL_NAMING
    // clang-format off
    const T& get() const { return Get(); }
    void set(const T& value) { Set(value); }
    auto& property_changed() { return PropertyChanged; }
    const auto& property_changed() const { return PropertyChanged; }
// clang-format on
#endif // USE_STL_NAMING
private:
    T m_value;
    bool m_isInitialized;
};

#ifdef USE_STL_NAMING
// clang-format off
template <typename T>
using prop_ch_ev_args = PropertyChangedEventArgs<T>;

template <typename T>
using property = Property<T>;
// clang-format on
#endif // USE_STL_NAMING

} // namespace vwr

#endif // CWEL_PROPERTY_HPP_
