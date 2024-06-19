#ifndef VWRTK_CWEL_EVENT_HPP
#define VWRTK_CWEL_EVENT_HPP

#include "Object.hpp"
#include <vector>

namespace vwr
{
    class EventArgs : public Object
    {
    public:
        virtual ~EventArgs() = default;
        static EventArgs Empty;

    protected:
        EventArgs() = default;
    };

    template<std::derived_from<EventArgs> TEventArgs>
    class Event : public Object
    {
        using delegate = void(*)(Object*, TEventArgs*);
    public:
        explicit Event(const Object& parent);
        void operator+=(delegate handler);
        void operator-=(delegate handler);
        void Invoke(Object* sender, TEventArgs* e);

    protected:
        const Object* m_parent;
        std::vector<delegate> m_handlers;
    };
}

#endif //VWRTK_CWEL_EVENT_HPP
