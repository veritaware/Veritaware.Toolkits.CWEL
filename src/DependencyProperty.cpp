#include "DependencyProperty.hpp"

template<class T>
vwr::ChangedEventArgs<T>::ChangedEventArgs(T* oldValue, T* newValue){
    m_oldValue = oldValue;
    m_newValue = newValue;
}

template<class T>
vwr::DependencyProperty<T>::DependencyProperty(const T& value) {
    _oldValue = value;
    _value = value;

    RaiseChanged();
}

template<class T>
const T& vwr::DependencyProperty<T>::Get() {
    return _value;
}

template<class T>
void vwr::DependencyProperty<T>::Set(const T& value) {
    _oldValue = _value;
    _value = value;

    auto* obj = (Object*)(&value);
    bool changed = obj ? obj->Equals((IGetHashCode)_value) : (value == _value);

    if(changed){
        RaiseChanged();
    }
}

template<class T>
vwr::DependencyProperty<T> &vwr::DependencyProperty<T>::operator=(const vwr::DependencyProperty<T> &other) {
    Set(other.Get());
    return *this;
}

template<class T>
vwr::DependencyProperty<T> &vwr::DependencyProperty<T>::operator=(const T &value) {
    Set(value);
    return *this;
}

template<class T>
void vwr::DependencyProperty<T>::RaiseChanged() {
    auto e = ChangedEventArgs<T>(&_oldValue, &_value);
    Changed.Invoke(this, e);
}
