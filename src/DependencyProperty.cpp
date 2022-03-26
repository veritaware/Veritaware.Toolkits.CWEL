#include "DependencyProperty.hpp"

template<class T>
vwr::PropertyChangedEventArgs<T>::PropertyChangedEventArgs(T* oldValue, T* newValue){
    m_oldValue = oldValue;
    m_newValue = newValue;
}

template<class T>
vwr::DependencyProperty<T>::DependencyProperty(T value) {
    _oldValue = value;
    _value = value;

    RaisePropertyChanged();
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
        RaisePropertyChanged();
    }
}

template<class T>
void vwr::DependencyProperty<T>::RaisePropertyChanged() {
    auto e = PropertyChangedEventArgs<T>(&_oldValue, &_value);
    PropertyChanged.Invoke(this, e);
}
