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
    const T old_value;
    const T new_value;
};

template <typename T>
class property
{
public:
    property() : property_changed(), m_value(), is_initialized(false) {}
    explicit property(const T& value) : property_changed(), m_value(value), is_initialized(true) {}

    /// Copies the value from other property without copying the event handlers as those are not copyable/movable.
    explicit property(const property& other) : property_changed(), m_value(other.m_value), is_initialized(other.is_initialized) {}

    /// Copies the value from other property without copying the event handlers as those are not copyable/movable
    /// and raises the property_changed event if the value has changed.
    property& operator=(const property& other)
    {
        if(this != &other)
            set(other.m_value);
        return *this;
    }

    /// Disabled due to event handlers not being movable.
    explicit property(property&& other) = delete;
    /// Disabled due to event handlers not being movable.
    property& operator=(property&& other) = delete;

    /// Returns the property value.
    const T& get() const { return m_value; }

    /// Returns the property value.
    const T& operator()() const { return m_value; }

    /// Sets the property value and raises the property_changed event if the value has changed.
    /// The property_changed event is not raised if the value is being initialized for the first time.
    void set(const T& value)
    {
        if(!is_initialized)
        {
            m_value = value;
            return;
        }
        if(m_value == value)
            return;
        property_changed_event_args<T> event_args(m_value, value);
        m_value = value;
        property_changed(this, event_args);
    }

    /// Sets the property value and raises the property_changed event if the value has changed.
    /// The property_changed event is not raised if the value is being initialized for the first time.
    property& operator=(const T& value)
    {
        set(value);
        return *this;
    }

    /// Event called when the property value has changed from its previous state.
    /// The property value must NOT be modified in the property_changed event callback, as it will cause a deadlock.
    event_handler<property_changed_event_args<T>> property_changed;
private:
    T m_value;
    bool is_initialized;
};

} // namespace vwr

#endif // CWEL_PROPERTY_HPP_
