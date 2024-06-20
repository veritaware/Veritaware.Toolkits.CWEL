#ifndef VWRTK_CWEL_PROPERTY_HPP
#define VWRTK_CWEL_PROPERTY_HPP

#include "Object.hpp"
#include "Event.hpp"

namespace vwr
{
    class ValueChangedEventArgs : public EventArgs
    {
        public:
        ValueChangedEventArgs() = default;
        explicit ValueChangedEventArgs(const Object& oldValue, const Object& newValue)
            : m_oldValue(oldValue), m_newValue(newValue) {}
        virtual ~ValueChangedEventArgs() = default;

        Object GetOldValue() const { return m_oldValue; }
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

    template<std::derived_from<Object> T>
    class Property : Object
    {
        public:
        Property();
        explicit Property(const T& value) : m_value(value) {}
        virtual ~Property() = default;

        T GetValue() const { return m_value; }
        void SetValue(const T& value);

        void operator=(const T& value) { SetValue(value); }
        T operator()() const { return GetValue(); }
        void operator<<(const T& value) { SetValue(value); }
        void operator>>(T& value) { value = GetValue(); }

        ValueChangedEvent ValueChanged;

        protected:
        T m_value;
    };
}

#endif //VWRTK_CWEL_PROPERTY_HPP
