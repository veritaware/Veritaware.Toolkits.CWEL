#include "TimeSpan.hpp"

#include <limits>

namespace vwr
{
    const vwr::TimeSpan vwr::TimeSpan::Zero = vwr::TimeSpan(0);
    const vwr::TimeSpan vwr::TimeSpan::MinValue = vwr::TimeSpan(std::numeric_limits<int64_t>::min());
    const vwr::TimeSpan vwr::TimeSpan::MaxValue = vwr::TimeSpan(std::numeric_limits<int64_t>::max());

    int64_t TimeSpan::TimeToTicks(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds)
    {
        int64_t total = 0;
        total += days * TicksPerDay;
        total += hours * TicksPerHour;
        total += minutes * TicksPerMinute;
        total += seconds * TicksPerSecond;
        total += milliseconds * TicksPerMillisecond;
        total += microseconds * 10;
        return total;
    }

    TimeSpan::TimeSpan(int32_t hours, int32_t minutes, int32_t seconds)
        : m_ticks(TimeToTicks(0, hours, minutes, seconds, 0, 0)) {}

    TimeSpan::TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds)
        : m_ticks(TimeToTicks(days, hours, minutes, seconds, 0, 0)) {}

    TimeSpan::TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds)
        : m_ticks(TimeToTicks(days, hours, minutes, seconds, milliseconds, 0)) {}

    TimeSpan::TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds)
        : m_ticks(TimeToTicks(days, hours, minutes, seconds, milliseconds, microseconds)) {}

    TimeSpan TimeSpan::Add(const TimeSpan& ts) const
    {
        int64_t ticks = m_ticks + ts.m_ticks;
        if((m_ticks >> 63) == (ts.m_ticks >> 63) && (m_ticks >> 63) != (ticks >> 63))
            throw std::overflow_error("Overflow in TimeSpan addition.");
        return TimeSpan(ticks);
    }

    TimeSpan TimeSpan::Subtract(const TimeSpan& ts) const
    {
        int64_t ticks = m_ticks - ts.m_ticks;
        if((m_ticks >> 63) != (ts.m_ticks >> 63) && (m_ticks >> 63) != (ticks >> 63))
            throw std::overflow_error("Overflow in TimeSpan subtraction.");
        return TimeSpan(ticks);
    }


}
