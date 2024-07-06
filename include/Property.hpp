#ifndef VWRTK_CWEL_PROPERTY_HPP
#define VWRTK_CWEL_PROPERTY_HPP

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
        explicit ValueChangedEventArgs(const Object& oldValue, const Object& newValue)
            : m_oldValue(oldValue), m_newValue(newValue) {}
        virtual ~ValueChangedEventArgs() = default;

        /// <summary>
        /// Gets the old value of the property.
        /// </summary>
        Object GetOldValue() const { return m_oldValue; }
        /// <summary>
        /// Gets the new value of the property.
        /// </summary>
        Object GetNewValue() const { return m_newValue; }

        protected:
        Object m_oldValue;
        Object m_newValue;
    };

    class ValueChangedEvent : public Event<ValueChangedEventArgs>
    {
        public:
        explicit ValueChangedEvent(const Object& parent) : Event(parent) {}
        virtual ~ValueChangedEvent() = default;
    };

    /// <summary>
    /// Represents a property that can be observed for changes.
    /// </summary>
    template<std::derived_from<Object> T>
    class Property : Object
    {
        public:
        Property();
        explicit Property(const T& value) : m_value(value) {}
        virtual ~Property() = default;

        T GetValue() const { return m_value; }
        void SetValue(const T& value);

        std::string ToString() const override { return m_value.ToString(); }

        void operator=(const T& value) { SetValue(value); }
        T operator()() const { return GetValue(); }
        void operator<<(const T& value) { SetValue(value); }
        void operator>>(T& value) { value = GetValue(); }

        /// <summary>
        /// Occurs when the value of the property changes.
        /// </summary>
        ValueChangedEvent ValueChanged;

        protected:
        T m_value;
    };
}

#endif //VWRTK_CWEL_PROPERTY_HPP
