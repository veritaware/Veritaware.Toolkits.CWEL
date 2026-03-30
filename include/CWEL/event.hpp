/* Event handler class
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#ifndef CWEL_EVENT_HPP_
#define CWEL_EVENT_HPP_

#include <algorithm>
#include <memory>
#include <mutex>
#include <vector>

namespace vwr
{

template <typename event_args_t>
class event_handler;

template <typename event_args_t>
class delegate
{
public:
    using subscriber = void(*)(const void*, const event_args_t&);
    explicit delegate(const subscriber callback) : m_handler(nullptr), m_callback(callback) {}

    ~delegate()
    {
        if(m_handler)
        {
            std::lock_guard lock(m_handler->m_mutex);
            m_handler->m_delegates.erase(
                std::remove(m_handler->m_delegates.begin(), m_handler->m_delegates.end(), this),
                m_handler->m_delegates.end()
            );
        }
    }

private:
    friend class event_handler<event_args_t>;
    event_handler<event_args_t>* m_handler;
    subscriber m_callback;
    void register_handler(event_handler<event_args_t>& handler) { m_handler = &handler; }
    void invoke(const void* sender, const event_args_t& event_args) { if(m_callback) m_callback(sender, event_args); }
};

template <typename event_args_t>
class event_handler
{
public:
    using subscriber = void(*)(const void*, const event_args_t&);

    event_handler& operator+=(delegate<event_args_t>& d)
    {
        std::lock_guard lock(m_mutex);
        if(std::find(m_delegates.begin(), m_delegates.end(), &d) == m_delegates.end())
            m_delegates.push_back(&d);
        return *this;
    }

    event_handler& operator-=(delegate<event_args_t>& d)
    {
        std::lock_guard lock(m_mutex);
        m_delegates.erase(
            std::remove(m_delegates.begin(), m_delegates.end(), &d),
            m_delegates.end()
        );
        return *this;
    }

    void operator()(const void* sender, const event_args_t& event_args)
    {
        // Prevents modification of the delegate list while invoking
        std::vector<delegate<event_args_t>*> snapshot;
        {
            std::lock_guard lock(m_mutex);
            snapshot = m_delegates;
        }
        for(auto d : snapshot)
            if(d) d->invoke(sender, event_args);
    }

private:
    friend class delegate<event_args_t>;
    std::mutex m_mutex;
    std::vector<delegate<event_args_t>*> m_delegates;
};

}

#endif //CWEL_EVENT_HPP_
