#include <algorithm>
#include "Event.hpp"

void vwr::EventHandler::Invoke(vwr::Object *sender, vwr::EventArgs &e) {
    for (auto& d : m_delegates)
    {
        (*d)(sender, e);
    }
}

vwr::EventHandler &vwr::EventHandler::operator+=(vwr::callback delegate) {
    m_delegates.push_back(delegate);
    return *this;
}

vwr::EventHandler &vwr::EventHandler::operator-=(vwr::callback delegate) {
    auto d = std::find(m_delegates.begin(), m_delegates.end(), delegate);

    if(d != m_delegates.end())
        m_delegates.erase(d);

    return *this;
}
