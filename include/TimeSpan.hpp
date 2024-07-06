#ifndef VWRTK_CWEL_TIMESPAN_HPP
#define VWRTK_CWEL_TIMESPAN_HPP

#include "Object.hpp"

#include <chrono>

namespace vwr
{
    /// <summary>
    /// Represents a time interval. Wraps over std::chrono::duration.
    /// </summary>
    class TimeSpan : public Object
    {
    public:
        /// <summary>
        /// Initializes a new instance of the TimeSpan structure to a specified number of hours, minutes, and seconds.
        /// </summary>
        TimeSpan(int32_t hours, int32_t minutes, int32_t seconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan structure to a specified number of days, hours, minutes, and seconds.
        /// </summary>
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan structure to a specified number of days, hours, minutes, seconds, and milliseconds.
        /// </summary>
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan structure to a specified number of days, hours, minutes, seconds, milliseconds, and microseconds.
        /// </summary>
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan structure to the specified number of ticks.
        /// </summary>
        explicit TimeSpan(int64_t ticks) : m_ticks(ticks) {}

        static const TimeSpan Zero;
        static const TimeSpan MinValue;
        static const TimeSpan MaxValue;

        int64_t Ticks() const { return m_ticks; }
        int32_t Days() const { return static_cast<int32_t>(m_ticks / TicksPerDay); }
        int32_t Hours() const { return static_cast<int32_t>((m_ticks / TicksPerHour) % 24); }
        int32_t Minutes() const { return static_cast<int32_t>((m_ticks / TicksPerMinute) % 60); }
        int32_t Seconds() const { return static_cast<int32_t>((m_ticks / TicksPerSecond) % 60); }

        double TotalDays() const { return static_cast<double>(m_ticks) * DaysPerTick; }
        double TotalHours() const { return static_cast<double>(m_ticks) * HoursPerTick; }
        double TotalMinutes() const { return static_cast<double>(m_ticks) * MinutesPerTick; }
        double TotalSeconds() const { return static_cast<double>(m_ticks) * SecondsPerTick; }
        double TotalMilliseconds() const { return static_cast<double>(m_ticks) * MillisecondsPerTick; }

        TimeSpan Add(const TimeSpan& ts) const ;
        TimeSpan Subtract(const TimeSpan& ts) const ;
        TimeSpan operator+(const TimeSpan& ts) const { return Add(ts); }
        TimeSpan operator-(const TimeSpan& ts) const { return Subtract(ts); }

        static const int64_t NanosecondsPerTick  = 100;
        static const int64_t TicksPerMicrosecond = 10;
        static const int64_t TicksPerMillisecond = TicksPerMicrosecond * 1000; // 10,000
        static const int64_t TicksPerSecond      = TicksPerMillisecond * 1000; // 1,000,000
        static const int64_t TicksPerMinute      = TicksPerSecond      * 60;   // 60,000,000
        static const int64_t TicksPerHour        = TicksPerMinute      * 60;   // 3,600,000,000
        static const int64_t TicksPerDay         = TicksPerHour        * 24;   // 86,400,000,000

    private:
        int64_t m_ticks;

        /// <summary>
        /// Returns the number of ticks from the specified days, hours, minutes, seconds, milliseconds, and microseconds.
        /// </summary>
        static int64_t TimeToTicks(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds);

        constexpr static const double MillisecondsPerTick = 1.0 / TicksPerMillisecond;
        constexpr static const double SecondsPerTick      = 1.0 / TicksPerSecond;
        constexpr static const double MinutesPerTick      = 1.0 / TicksPerMinute;
        constexpr static const double HoursPerTick        = 1.0 / TicksPerHour;
        constexpr static const double DaysPerTick         = 1.0 / TicksPerDay;

        static const int64_t MaxSeconds = std::numeric_limits<int64_t>::max() / TicksPerSecond;
        static const int64_t MinSeconds = std::numeric_limits<int64_t>::min() / TicksPerSecond;

        static const int64_t MaxMilliSeconds = std::numeric_limits<int64_t>::max() / TicksPerMillisecond;
        static const int64_t MinMilliSeconds = std::numeric_limits<int64_t>::min() / TicksPerMillisecond;

        static const int64_t TicksPerTenthSecond = TicksPerMillisecond * 100;
    };
}

#endif //VWRTK_CWEL_TIMESPAN_HPP
