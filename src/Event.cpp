#include "Event.hpp"

namespace vwr
{
#define TMPL template<std::derived_from<EventArgs> TEventArgs>
#define EVENT_T Event<TEventArgs>

    TMPL EVENT_T::Event(const Object& parent) : m_parent(&parent) { }

    TMPL void EVENT_T::operator+=(EVENT_T::delegate handler)
    {
        m_handlers.push_back(handler);
    }

    TMPL void EVENT_T::operator-=(EVENT_T::delegate handler)
    {
        m_handlers.Remove(handler);
    }

    TMPL void EVENT_T::Invoke(Object* sender, TEventArgs* e) // NOLINT(*-identifier-length)
    {
        if (sender == nullptr)
        {
            throw std::invalid_argument("Sender cannot be null.");
        }

        if (m_parent != sender)
        {
            throw std::invalid_argument("Sender is not the parent object of this event.");
        }

        for (auto handler : m_handlers)
        {
            handler(sender, e);
        }
    }

#undef TMPL
#undef EVENT_T
}
