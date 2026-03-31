/* Event handler class
 * Copyright (c) 2026 Veritaware
 * SPDX-License-Identifier: Zlib
 */

#ifndef CWEL_EVENT_HPP_
#define CWEL_EVENT_HPP_

#include <algorithm>
#include <mutex>
#include <stdexcept>
#include <vector>

namespace vwr
{

template <typename event_args_t>
class event_handler;

/// Provides a delegate type that can be used to subscribe callbacks to events with specific argument types.
/// Copying/moving delegates is not permitted.
template <typename event_args_t>
class delegate
{
public:
    using subscriber = void(*)(const void*, const event_args_t&);
    explicit delegate(const subscriber callback) : m_handler(nullptr), m_callback(callback) {}
    delegate(const delegate&) = delete;
    delegate& operator=(const delegate&) = delete;
    delegate(delegate&& other) = delete;
    delegate& operator=(delegate&& other) = delete;

    /// Automatically deregisters the delegate from its event handler upon destruction to prevent dangling pointers.
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

    /// Registers the event handler for future invocations and stores a pointer to it for automatic deregistration.
    void register_handler(event_handler<event_args_t>& handler)
    {
        if (m_handler)
            throw std::logic_error("delegate already registered");
        m_handler = &handler;
    }

    /// Deregisters the event handler to prevent future invocations and clears the stored pointer.
    void deregister_handler() { m_handler = nullptr; }

    /// Invokes the callback with the provided sender and event arguments if the callback is valid.
    void invoke(const void* sender, const event_args_t& event_args) { if(m_callback) m_callback(sender, event_args); }
};

/// Event handler class that supports callbacks with specific event_args_t argument parameter.
template <typename event_args_t>
class event_handler
{
public:
    ~event_handler()
    {
        std::scoped_lock lock(m_mutex);
        for(auto d : m_delegates)
            if(d) d->deregister_handler();
    }

    /// Adds a delegate to the event handler, ensuring that it is not added multiple times.
    /// The delegate will be automatically deregistered upon destruction.
    event_handler& operator+=(delegate<event_args_t>& d)
    {
        std::scoped_lock lock(m_mutex);
        if(std::find(m_delegates.begin(), m_delegates.end(), &d) == m_delegates.end())
        {
            m_delegates.push_back(&d);
            d.register_handler(*this);
        }
        return *this;
    }

    /// Removes a delegate from the event handler, preventing future invocations of the callback.
    event_handler& operator-=(delegate<event_args_t>& d)
    {
        std::scoped_lock lock(m_mutex);
        const auto it = std::find(m_delegates.begin(), m_delegates.end(), &d);
        if(it != m_delegates.end())
        {
            d.deregister_handler();
            m_delegates.erase(it);
        }
        return *this;
    }

    /// Invokes all registered delegates with the provided sender and event arguments.
    /// Callbacks can't subscribe/unsubscribe during invocation
    void operator()(const void* sender, const event_args_t& event_args)
    {
        // Hold the mutex for the entire duration of the invocation to prevent
        // concurrent modification or destruction of delegates while invoking.
        std::scoped_lock lock(m_mutex);
        for(auto d : m_delegates)
            if(d) d->invoke(sender, event_args);
    }

private:
    friend class delegate<event_args_t>;
    std::mutex m_mutex;
    std::vector<delegate<event_args_t>*> m_delegates;
};

} // namespace vwr

#endif // CWEL_EVENT_HPP_
