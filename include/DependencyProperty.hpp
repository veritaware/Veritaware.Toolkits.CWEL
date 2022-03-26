#ifndef VERITAWARE_TOOLKITS_CWEL_DEPENDENCYPROPERTY_
#define VERITAWARE_TOOLKITS_CWEL_DEPENDENCYPROPERTY_

#include "Object.hpp"
#include "Event.hpp"

namespace vwr {
    template<class T>
    class ChangedEventArgs : public EventArgs {
    public:
        ChangedEventArgs(T* oldValue, T* newValue);

        std::weak_ptr<T> OldValue() { return m_oldValue; }
        std::weak_ptr<T> NewValue() { return m_newValue; }

    private:
        T* m_oldValue;
        T* m_newValue;
    };

    template<class T>
    class ChangedEventHandler : public EventHandler {
    public:
        void Invoke(Object* sender, ChangedEventArgs<T>& e) { EventHandler::Invoke(sender, e); }

        typedef void(*callback)(Object* sender, ChangedEventArgs<T>& e);

        ChangedEventHandler& operator +=(callback delegate){ EventHandler::operator+=(delegate); }
        ChangedEventHandler& operator -=(callback delegate){ EventHandler::operator-=(delegate); }
    };

    template<class T>
    class DependencyProperty {
    public:
        DependencyProperty() = default;
        explicit DependencyProperty(const T& value);

        const T& Get();
        void Set(const T& value);

        const T& operator()() { return Get(); }
        void operator()(const T& value) { Set(value); }

        DependencyProperty<T>& operator=(const DependencyProperty<T>& other);
        DependencyProperty<T>& operator=(const T& value);

        ChangedEventHandler<T> Changed;

    private:
        T _value;
        T _oldValue;

        void RaiseChanged();
    };

#define prop DependencyProperty
}

#endif //VERITAWARE_TOOLKITS_CWEL_DEPENDENCYPROPERTY_
