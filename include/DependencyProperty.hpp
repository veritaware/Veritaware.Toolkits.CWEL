#ifndef VERITAWARE_TOOLKITS_CWEL_DEPENDENCYPROPERTY_
#define VERITAWARE_TOOLKITS_CWEL_DEPENDENCYPROPERTY_

#include "Object.hpp"
#include "Event.hpp"

namespace vwr {
    template<class T>
    class PropertyChangedEventArgs : public EventArgs {
    public:
        PropertyChangedEventArgs(T* oldValue, T* newValue);

        std::weak_ptr<T> OldValue() { return m_oldValue; }
        std::weak_ptr<T> NewValue() { return m_newValue; }

    private:
        T* m_oldValue;
        T* m_newValue;
    };

    template<class T>
    class PropertyChangedEventHandler : public EventHandler {
    public:
        void Invoke(Object* sender, PropertyChangedEventArgs<T>& e) { EventHandler::Invoke(sender, e); }

        typedef void(*callback)(Object* sender, PropertyChangedEventArgs<T>& e);

        PropertyChangedEventHandler& operator +=(callback delegate){ EventHandler::operator+=(delegate); }
        PropertyChangedEventHandler& operator -=(callback delegate){ EventHandler::operator-=(delegate); }
    };

    template<class T>
    class DependencyProperty {
    public:
        DependencyProperty() = default;
        explicit DependencyProperty(T value);

        const T& Get();
        void Set(const T& value);

        const T& operator()() { return Get(); }
        void operator()(const T& value) { Set(value); }

        PropertyChangedEventHandler<T> PropertyChanged;

    private:
        T _value;
        T _oldValue;

        void RaisePropertyChanged();
    };

#define prop DependencyProperty
}

#endif //VERITAWARE_TOOLKITS_CWEL_DEPENDENCYPROPERTY_
