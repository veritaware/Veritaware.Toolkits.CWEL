#ifndef VWRTK_CWEL_TIMESPAN_HPP
#define VWRTK_CWEL_TIMESPAN_HPP

#include "Object.hpp"
#include "IComparable.hpp"

#include <chrono>

namespace vwr
{
    /// <summary>
    /// Represents a time interval. Wraps over std::chrono::duration.
    /// </summary>
    class TimeSpan : public Object, public IComparable, public IComparableT<TimeSpan>
    {
    public:
        /// <summary>
        /// Initializes a new instance of the TimeSpan class to a specified number of hours, minutes, and seconds.
        /// </summary>
        TimeSpan(int32_t hours, int32_t minutes, int32_t seconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan class to a specified number of days, hours, minutes, and seconds.
        /// </summary>
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan class to a specified number of days, hours, minutes, seconds, and milliseconds.
        /// </summary>
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan class to a specified number of days, hours, minutes, seconds, milliseconds, and microseconds.
        /// </summary>
        TimeSpan(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds);
        /// <summary>
        /// Initializes a new instance of the TimeSpan class to the specified number of ticks.
        /// </summary>
        explicit TimeSpan(int64_t ticks) : m_ticks(ticks) {}
        /// <summary>
        /// Initializes a new instance of the TimeSpan class from a std::chrono::duration.
        /// </summary>
        explicit TimeSpan(const std::chrono::nanoseconds& ns) : m_ticks(ns.count() / NanosecondsPerTick) {} // NOLINT(*-identifier-length)

        [[nodiscard]] int8_t CompareTo(const Object& value) const override;
        [[nodiscard]] int8_t CompareTo(const TimeSpan& value) const override;

        [[nodiscard]] TimeSpan Add(const TimeSpan& span) const ;
        [[nodiscard]] TimeSpan Subtract(const TimeSpan& span) const ;
        TimeSpan operator+(const TimeSpan& span) const { return Add(span); }
        TimeSpan operator-(const TimeSpan& span) const { return Subtract(span); }

        static const TimeSpan Zero;
        static const TimeSpan MinValue;
        static const TimeSpan MaxValue;

        [[nodiscard]] int64_t Ticks() const { return m_ticks; }
        [[nodiscard]] int32_t Days() const { return static_cast<int32_t>(m_ticks / TicksPerDay); }
        [[nodiscard]] int32_t Hours() const { return static_cast<int32_t>(m_ticks / TicksPerHour % HoursPerDay); }
        [[nodiscard]] int32_t Minutes() const { return static_cast<int32_t>(m_ticks / TicksPerMinute % MinutesPerHour); }
        [[nodiscard]] int32_t Seconds() const { return static_cast<int32_t>(m_ticks / TicksPerSecond % SecondsPerMinute); }

        [[nodiscard]] double TotalDays() const { return static_cast<double>(m_ticks) * DaysPerTick; }
        [[nodiscard]] double TotalHours() const { return static_cast<double>(m_ticks) * HoursPerTick; }
        [[nodiscard]] double TotalMinutes() const { return static_cast<double>(m_ticks) * MinutesPerTick; }
        [[nodiscard]] double TotalSeconds() const { return static_cast<double>(m_ticks) * SecondsPerTick; }
        [[nodiscard]] double TotalMilliseconds() const { return static_cast<double>(m_ticks) * MillisecondsPerTick; }
        [[nodiscard]] double TotalMicroseconds() const { return static_cast<double>(m_ticks) * MicrosecondsPerTick; }

        static constexpr int64_t NanosecondsPerTick  = 100;
        static constexpr int64_t TicksPerMicrosecond = 10;
        static constexpr int64_t TicksPerMillisecond = TicksPerMicrosecond * 1000; //         10,000
        static constexpr int64_t TicksPerSecond      = TicksPerMillisecond * 1000; //      1,000,000
        static constexpr int64_t TicksPerMinute      = TicksPerSecond      * 60;   //     60,000,000
        static constexpr int64_t TicksPerHour        = TicksPerMinute      * 60;   //  3,600,000,000
        static constexpr int64_t TicksPerDay         = TicksPerHour        * 24;   // 86,400,000,000

    private:
        int64_t m_ticks;
        static constexpr int32_t HoursPerDay = 24;
        static constexpr int32_t MinutesPerHour = 60;
        static constexpr int32_t SecondsPerMinute = 60;

        /// <summary>
        /// Returns the number of ticks from the specified days, hours, minutes, seconds, milliseconds, and microseconds.
        /// </summary>
        static int64_t TimeToTicks(int32_t days, int32_t hours, int32_t minutes, int32_t seconds, int32_t milliseconds, int32_t microseconds);

        static constexpr double MicrosecondsPerTick = 1.0 / TicksPerMicrosecond;
        static constexpr double MillisecondsPerTick = 1.0 / TicksPerMillisecond;
        static constexpr double SecondsPerTick      = 1.0 / TicksPerSecond;
        static constexpr double MinutesPerTick      = 1.0 / TicksPerMinute;
        static constexpr double HoursPerTick        = 1.0 / TicksPerHour;
        static constexpr double DaysPerTick         = 1.0 / TicksPerDay;

        static constexpr int64_t MaxSeconds = std::numeric_limits<int64_t>::max() / TicksPerSecond;
        static constexpr int64_t MinSeconds = std::numeric_limits<int64_t>::min() / TicksPerSecond;

        static constexpr int64_t MaxMilliSeconds = std::numeric_limits<int64_t>::max() / TicksPerMillisecond;
        static constexpr int64_t MinMilliSeconds = std::numeric_limits<int64_t>::min() / TicksPerMillisecond;

        static constexpr int64_t MaxMicroSeconds = std::numeric_limits<int64_t>::max() / TicksPerMicrosecond;
        static constexpr int64_t MinMicroSeconds = std::numeric_limits<int64_t>::min() / TicksPerMicrosecond;

        static constexpr int64_t TicksPerTenthSecond = TicksPerMillisecond * 100;
    };
}

#endif //VWRTK_CWEL_TIMESPAN_HPP
