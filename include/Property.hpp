#ifndef VWRTK_CWEL_PROPERTY_HPP
#define VWRTK_CWEL_PROPERTY_HPP

#include <utility>

#include "Object.hpp"
#include "Event.hpp"

namespace vwr
{
    /// <summary>
    /// Provides data for the ValueChanged event.
    /// </summary>
    class ValueChangedEventArgs : public EventArgs
    {
    public:
        ValueChangedEventArgs() = default;
        explicit ValueChangedEventArgs(Object oldValue, Object newValue)
            : m_oldValue(std::move(oldValue)), m_newValue(std::move(newValue)) {}
        ~ValueChangedEventArgs() override = default;

        /// <summary>
        /// Gets the old value of the property.
        /// </summary>
        [[nodiscard]] Object GetOldValue() const { return m_oldValue; }
        /// <summary>
        /// Gets the new value of the property.
        /// </summary>
        [[nodiscard]] Object GetNewValue() const { return m_newValue; }

    private:
        Object m_oldValue;
        Object m_newValue;
    };

    class ValueChangedEvent : public Event<ValueChangedEventArgs>
    {
    public:
        explicit ValueChangedEvent(const Object& parent) : Event(parent) {}
        ~ValueChangedEvent() override = default;
    };

    /// <summary>
    /// Represents a property that can be observed for changes.
    /// </summary>
    template<std::derived_from<Object> T>
    class Property : Object
    {
    public:
        Property() : ValueChanged(this), m_value() { }
        explicit Property(const T& value) : ValueChanged(this), m_value(value) { }
        ~Property() override = default;

        T GetValue() const { return m_value; }
        void SetValue(const T& value);

        [[nodiscard]] std::string ToString() const override { return m_value.ToString(); }

        Property& operator=(const T& value) { SetValue(value); return *this; }
        T operator()() const { return GetValue(); }
        void operator<<(const T& value) { SetValue(value); }
        void operator>>(T& value) { value = GetValue(); }

        /// <summary>
        /// Occurs when the value of the property changes.
        /// </summary>
        ValueChangedEvent ValueChanged; // NOLINT(*-non-private-member-variables-in-classes)

    private:
        T m_value;
    };
}

#endif //VWRTK_CWEL_PROPERTY_HPP
