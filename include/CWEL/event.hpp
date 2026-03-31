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

template <typename EventArgsT>
class EventHandler;

/// Provides a Delegate type that can be used to subscribe callbacks to events with specific argument types.
/// Copying/moving Delegates is not permitted.
template <typename EventArgsT>
class Delegate
{
public:
    using Subscriber = void(*)(const void*, const EventArgsT&);
    explicit Delegate(const Subscriber callback) : m_handler(nullptr), m_callback(callback) {}
    Delegate(const Delegate&) = delete;
    Delegate& operator=(const Delegate&) = delete;
    Delegate(Delegate&& other) = delete;
    Delegate& operator=(Delegate&& other) = delete;

    /// Automatically deregisters the Delegate from its EventHandler upon destruction to prevent dangling pointers.
    ~Delegate()
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
    friend class EventHandler<EventArgsT>;
    EventHandler<EventArgsT>* m_handler;
    Subscriber m_callback;

    /// Registers the EventHandler for future invocations and stores a pointer to it for automatic deregistration.
    void registerHandler(EventHandler<EventArgsT>& handler)
    {
        if(m_handler)
            throw std::logic_error("Delegate already registered");
        m_handler = &handler;
    }

    /// Deregisters the EventHandler to prevent future invocations and clears the stored pointer.
    void deregisterHandler() { m_handler = nullptr; }

    /// Invokes the callback with the provided sender and event arguments if the callback is valid.
    void invoke(const void* sender, const EventArgsT& eventArgs) { if(m_callback) m_callback(sender, eventArgs); }
};

/// EventHandler class that supports callbacks with a specific EventArgsT argument parameter.
template <typename EventArgsT>
class EventHandler
{
public:
    ~EventHandler()
    {
        std::scoped_lock lock(m_mutex);
        for(auto d : m_delegates)
            if(d) d->deregisterHandler();
    }

    /// Adds a Delegate to the EventHandler, ensuring that it is not added multiple times.
    /// The Delegate will be automatically deregistered upon destruction.
    EventHandler& operator+=(Delegate<EventArgsT>& d)
    {
        std::scoped_lock lock(m_mutex);
        if(std::find(m_delegates.begin(), m_delegates.end(), &d) == m_delegates.end())
        {
            m_delegates.push_back(&d);
            d.registerHandler(*this);
        }
        return *this;
    }

    /// Removes a Delegate from the EventHandler, preventing future invocations of the callback.
    EventHandler& operator-=(Delegate<EventArgsT>& d)
    {
        std::scoped_lock lock(m_mutex);
        const auto it = std::find(m_delegates.begin(), m_delegates.end(), &d);
        if(it != m_delegates.end())
        {
            d.deregisterHandler();
            m_delegates.erase(it);
        }
        return *this;
    }

    /// Invokes all registered Delegates with the provided sender and event arguments.
    /// Callbacks cannot subscribe or unsubscribe during invocation.
    void operator()(const void* sender, const EventArgsT& eventArgs)
    {
        // Hold the mutex for the entire duration of the invocation to prevent
        // concurrent modification or destruction of Delegates while invoking.
        std::scoped_lock lock(m_mutex);
        for(auto d : m_delegates)
            if(d) d->invoke(sender, eventArgs);
    }

private:
    friend class Delegate<EventArgsT>;
    std::mutex m_mutex;
    std::vector<Delegate<EventArgsT>*> m_delegates;
};

#ifdef USE_STL_NAMING
template <typename ev_args_t>
using ev_hndlr = EventHandler<ev_args_t>;

template <typename ev_args_t>
using delegate = Delegate<ev_args_t>;
#endif // USE_STL_NAMING

} // namespace vwr

#endif // CWEL_EVENT_HPP_
