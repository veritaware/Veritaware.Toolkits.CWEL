#ifndef VWRTK_CWEL_OBJECT_HPP
#define VWRTK_CWEL_OBJECT_HPP

#include "IGetHashCode.hpp"
#include "IToString.hpp"

// ReSharper disable once CppUnusedIncludeDirective
#include <concepts>
#include <typeinfo>

namespace vwr
{
    /// <summary>
    /// Serves as the base class for all Veritaware Toolkit classes.
    /// </summary>
    class Object : //NOLINT
        public IGetHashCode, 
        public IToString 
    {
    public:
        Object() = default;
        [[nodiscard]] int32_t GetHashCode() const override;
        /// <summary>
        /// Returns a string that represents the current object.
        /// </summary>
        [[nodiscard]] std::string ToString() const override;
        /// <summary>
        /// Returns the type of current instance.
        /// </summary>
        [[nodiscard]] const std::type_info& GetType() const;
        /// <summary>
        /// Returns a string representing the name of the current type.
        /// </summary>
        [[nodiscard]] std::string GetTypeName() const;

        /// <summary>
        /// Determines whether the given object is equal to the current object.
        /// </summary>
        [[nodiscard]] bool Equals(const IGetHashCode &obj) const;
        /// <summary>
        /// Determines whether two given object instances are equal.
        /// </summary>
        [[nodiscard]] static bool Equals(const IGetHashCode &obj1, const IGetHashCode &obj2);
    };

    class Boolean;
    class SByte;
    class Byte;
    class Int16;
    class UInt16;
    class Int32;
    class UInt32;
    class Int64;
    class UInt64;
    class Single;
    class Double;

    /// <summary>
    /// Provides OOP wrapper for integral and floating-point types.
    /// </summary>
    template <typename T>
    class ValueType : public Object
    {
    public:
        [[nodiscard]] T GetValue() const { return m_value; }
        void SetValue(T value) { m_value = value; }
        [[nodiscard]] std::string ToString() const override { return std::to_string(m_value); }
    private:
        ValueType() = default;
        explicit ValueType(T value) : m_value(value) {}
        T m_value;

        friend class Boolean;
        friend class SByte;
        friend class Byte;
        friend class Int16;
        friend class UInt16;
        friend class Int32;
        friend class UInt32;
        friend class Int64;
        friend class UInt64;
        friend class Single;
        friend class Double;
    };

    class Boolean final : public ValueType<bool>
    {
    public:
        Boolean() : ValueType(false) {}
        explicit Boolean(const bool value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return m_value ? 1 : 0;
        }
        [[nodiscard]] std::string ToString() const override
        {
            return m_value ? "True" : "False";
        }
    };

    class SByte final : public ValueType<int8_t>
    {
    public:
        SByte() : ValueType(0) {}
        explicit SByte(const int8_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value)
                 | static_cast<int32_t>(m_value) << ShortShift;
        }
    private:
        static constexpr int8_t ShortShift = 16;
    };

    class Byte final : public ValueType<uint8_t>
    {
    public:
        Byte() : ValueType(0) {}
        explicit Byte(const uint8_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return m_value;
        }
    };

    class Int16 final : public ValueType<int16_t>
    {
    public:
        Int16() : ValueType(0) {}
        explicit Int16(const int16_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return (static_cast<int32_t>(m_value) << ShortShift)
                 | static_cast<uint16_t>(m_value);
        }
    private:
        static constexpr int8_t ShortShift = 16;
    };

    class UInt16 final : public ValueType<uint16_t>
    {
    public:
        UInt16() : ValueType(0) {}
        explicit UInt16(const uint16_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return m_value;
        }
    };

    class Int32 final : public ValueType<int32_t>
    {
    public:
        Int32() : ValueType(0) {}
        explicit Int32(const int32_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return m_value;
        }
    };

    class UInt32 final : public ValueType<uint32_t>
    {
    public:
        UInt32() : ValueType(0) {}
        explicit UInt32(const uint32_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value);
        }
    };

    class Int64 final : public ValueType<int64_t>
    {
    public:
        Int64() : ValueType(0) {}
        explicit Int64(const int64_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value)
                 ^ static_cast<int32_t>(m_value >> LongShift);
        }
    private:
        static constexpr int8_t LongShift = 32;
    };

    class UInt64 final : public ValueType<uint64_t>
    {
    public:
        UInt64() : ValueType(0) {}
        explicit UInt64(const uint64_t value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value)
                 ^ static_cast<int32_t>(m_value >> LongShift);
        }
    private:
        static constexpr int8_t LongShift = 32;
    };

    class Single final : public ValueType<float>
    {
    public:
        Single() : ValueType(0.0F) {}
        explicit Single(const float value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            if (m_value == 0.0F)
            {
                return 0;
            }

            return *reinterpret_cast<const int32_t*>(&m_value);
        }

    };

    class Double final : public ValueType<double>
    {
    public:
        Double() : ValueType(0.0) {}
        explicit Double(const double value) : ValueType(value) {}
        [[nodiscard]] int32_t GetHashCode() const override
        {
            if (m_value == 0.0)
            {
                return 0;
            }

            int64_t const value = *reinterpret_cast<const int64_t*>(&m_value);
            return static_cast<int32_t>(value)
                 ^ static_cast<int32_t>(value >> LongShift);
        }
    private:
        static constexpr int8_t LongShift = 32;
    };
}

#endif  //VWRTK_CWEL_OBJECT_HPP
