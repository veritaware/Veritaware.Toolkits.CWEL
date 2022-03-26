#ifndef VERITAWARE_TOOLKITS_CWEL_EVENT_
#define VERITAWARE_TOOLKITS_CWEL_EVENT_

#include <vector>
#include "Object.hpp"

namespace vwr {
    class EventArgs : public Object {};


    class EventHandler : public Object {
    public:
        EventHandler() = default;

        void Invoke(Object* sender, EventArgs& e);

        typedef void(*callback)(Object* sender, EventArgs& e);

        EventHandler& operator +=(callback delegate);
        EventHandler& operator -=(callback delegate);

    protected:
        EventHandler& operator =(const EventHandler& e) { return *this; }; // non-re-assignable!
        std::vector<callback> m_delegates{};
    };
}

#endif //VERITAWARE_TOOLKITS_CWEL_EVENT_
