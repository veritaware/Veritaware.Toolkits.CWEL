#include "Property.hpp"

namespace vwr
{
    template<std::derived_from<Object> T>
    Property<T>::Property()
    {
        ValueChanged = ValueChangedEvent(*this);
    }

    template<std::derived_from<Object> T>
    void Property<T>::SetValue(const T& value)
    {
        if(value.Equals(m_value))
            return;

        T oldValue = m_value;
        m_value = value;
        ValueChanged.Invoke(this, new ValueChangedEventArgs(oldValue, m_value));
    }
}
