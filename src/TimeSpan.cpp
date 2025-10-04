#include "TimeSpan.hpp"

// ReSharper disable once CppUnusedIncludeDirective
#include <cstddef>
#include <limits>

namespace vwr
{
    const TimeSpan TimeSpan::Zero = TimeSpan(0); // NOLINT(*-err58-cpp)
    const TimeSpan TimeSpan::MinValue = TimeSpan(std::numeric_limits<int64_t>::min()); // NOLINT(*-err58-cpp)
    const TimeSpan TimeSpan::MaxValue = TimeSpan(std::numeric_limits<int64_t>::max()); // NOLINT(*-err58-cpp)

    // >> 63 gives the sign bit (either 64 1's or 64 0's).
    static constexpr int64_t SignShift = 63;

    int64_t TimeSpan::TimeToTicks(
        const int32_t days, // NOLINT(*-easily-swappable-parameters)
        const int32_t hours,
        const int32_t minutes,
        const int32_t seconds,
        const int32_t milliseconds,
        const int32_t microseconds)
    {
        constexpr int32_t usInTick = 10;
        int64_t total = 0;
        total += days * TicksPerDay;
        total += hours * TicksPerHour;
        total += minutes * TicksPerMinute;
        total += seconds * TicksPerSecond;
        total += milliseconds * TicksPerMillisecond;
        total += static_cast<int64_t>(microseconds * usInTick);
        return total;
    }

    TimeSpan::TimeSpan(const int32_t hours, const int32_t minutes, const int32_t seconds)
        : m_ticks(TimeToTicks(0, hours, minutes, seconds, 0, 0)) {}

    TimeSpan::TimeSpan(const int32_t days, const int32_t hours, const int32_t minutes, const int32_t seconds)
        : m_ticks(TimeToTicks(days, hours, minutes, seconds, 0, 0)) {}

    TimeSpan::TimeSpan(const int32_t days, const int32_t hours, const int32_t minutes, const int32_t seconds, const int32_t milliseconds)
        : m_ticks(TimeToTicks(days, hours, minutes, seconds, milliseconds, 0)) {}

    TimeSpan::TimeSpan(const int32_t days, const int32_t hours, const int32_t minutes, const int32_t seconds, const int32_t milliseconds, const int32_t microseconds)
        : m_ticks(TimeToTicks(days, hours, minutes, seconds, milliseconds, microseconds)) {}

    int8_t TimeSpan::CompareTo(const Object& value) const
    {
        TimeSpan const* span = dynamic_cast<TimeSpan*>(const_cast<Object*>(&value));
        if(span == nullptr)
        {
            throw std::invalid_argument("Object is not a TimeSpan");
        }
        return CompareTo(*span);
    }

    int8_t TimeSpan::CompareTo(const TimeSpan& value) const
    {
        if (m_ticks < value.m_ticks) // NOLINT(*-braces-around-statements)
            return -1;
        if (m_ticks > value.m_ticks) // NOLINT(*-braces-around-statements)
            return 1;
        return 0;
    }

    TimeSpan TimeSpan::Add(const TimeSpan& span) const
    {
        int64_t const ticks = m_ticks + span.m_ticks;
        // Overflow if signs of operands were identical and result's sign
        // was opposite to the first argument's sign
        if(    m_ticks >> SignShift == span.m_ticks >> SignShift
            && m_ticks >> SignShift != ticks >> SignShift)
        {
            throw std::overflow_error("Overflow in TimeSpan addition.");
        }
        return TimeSpan(ticks);
    }

    TimeSpan TimeSpan::Subtract(const TimeSpan& span) const
    {
        int64_t const ticks = m_ticks - span.m_ticks;
        // Overflow if signs of operands were different and result's sign
        // was opposite to the first argument's sign
        if(    m_ticks >> SignShift != span.m_ticks >> SignShift
            && m_ticks >> SignShift != ticks >> SignShift)
        {
            throw std::overflow_error("Overflow in TimeSpan subtraction.");
        }
        return TimeSpan(ticks);
    }
}
