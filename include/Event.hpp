#ifndef VWRTK_CWEL_EVENT_HPP
#define VWRTK_CWEL_EVENT_HPP

#include "Object.hpp"
#include "Extensions/Vector.hpp"

namespace vwr
{
    /// <summary>
    /// Provides the base class for classes that contain event data,
    /// and a value to use for events without event data.
    /// </summary>
    class EventArgs : public Object
    {
    public:
        ~EventArgs() override = default;
        static EventArgs Empty;

    protected:
        EventArgs() = default;
    };

    /// <summary>
    /// Provides a class to notify other classes whenever something of interest occurs.
    /// Calls back a delegate method when the event is raised.
    /// </summary>
    template<std::derived_from<EventArgs> TEventArgs>
    class Event : public Object
    {
        /// <summary>
        /// Represents the method that handles an event.
        /// </summary>
        using delegate = void(*)(Object* sender, TEventArgs* e); // NOLINT(*-identifier-length)
    public:
        explicit Event(const Object& parent);
        /// <summary>
        /// Subscribes to the event invocation.
        /// </summary>
        void operator+=(delegate handler);
        /// <summary>
        /// Unsubscribes from the event invocation.
        /// </summary>
        void operator-=(delegate handler);
        /// <summary>
        /// Invokes event callbacks. Can be performed by the event's parent object only.
        /// </summary>
        void Invoke(Object* sender, TEventArgs* e); // NOLINT(*-identifier-length)

    private:
        const Object* m_parent;
        vector<delegate> m_handlers;
    };
}

#endif //VWRTK_CWEL_EVENT_HPP
