#ifndef VWRTK_CWEL_OBJECT_HPP
#define VWRTK_CWEL_OBJECT_HPP

#include "IGetHashCode.hpp"
#include "IToString.hpp"

namespace vwr
{
    class Object : //NOLINT
        public IGetHashCode, 
        public IToString 
    {
    public:
        Object() = default;
        [[nodiscard]] int32_t GetHashCode() const override;
        [[nodiscard]] std::string ToString() const override;
        [[nodiscard]] const std::type_info& GetType() const;

        [[nodiscard]] bool Equals(const IGetHashCode &obj) const;
        static bool Equals(const IGetHashCode &obj1, const IGetHashCode &obj2);
    };

    template <typename T>
    class ValueType : public Object
    {
    public:
        [[nodiscard]] T GetValue() const { return m_value; }
        void SetValue(T value) { m_value = value; }
    protected:
        ValueType() = default;
        explicit ValueType(T value) : m_value(value) {}
        T m_value;
    };

    class Boolean : public ValueType<bool>
    {
    public:
        Boolean() : ValueType(false) {}
        explicit Boolean(bool value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return m_value ? 1 : 0;
        }
    };

    class SByte : public ValueType<int8_t>
    {
    public:
        SByte() : ValueType(0) {}
        explicit SByte(int8_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value)
                 | static_cast<int32_t>(m_value) << 16;
        }
    };

    class Byte : public ValueType<uint8_t>
    {
    public:
        Byte() : ValueType(0) {}
        explicit Byte(uint8_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return m_value;
        }
    };

    class Int16 : public ValueType<int16_t>
    {
    public:
        Int16() : ValueType(0) {}
        explicit Int16(int16_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return (static_cast<int32_t>(m_value) << 16)
                 | static_cast<uint16_t>(m_value);
        }
    };

    class UInt16 : public ValueType<uint16_t>
    {
    public:
        UInt16() : ValueType(0) {}
        explicit UInt16(uint16_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value);
        }
    };

    class Int32 : public ValueType<int32_t>
    {
    public:
        Int32() : ValueType(0) {}
        explicit Int32(int32_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return m_value;
        }
    };

    class UInt32 : public ValueType<uint32_t>
    {
    public:
        UInt32() : ValueType(0) {}
        explicit UInt32(uint32_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value);
        }
    };

    class Int64 : public ValueType<int64_t>
    {
    public:
        Int64() : ValueType(0) {}
        explicit Int64(int64_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value)
                 ^ static_cast<int32_t>(m_value >> 32);
        }
    };

    class UInt64 : public ValueType<uint64_t>
    {
    public:
        UInt64() : ValueType(0) {}
        explicit UInt64(uint64_t value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            return static_cast<int32_t>(m_value)
                 ^ static_cast<int32_t>(m_value >> 32);
        }
    };

    class Single : public ValueType<float>
    {
    public:
        Single() : ValueType(0.0f) {}
        explicit Single(float value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            if (m_value == 0.0f)
                return 0;

            return *reinterpret_cast<const int32_t*>(&m_value);
        }

    };

    class Double : public ValueType<double>
    {
    public:
        Double() : ValueType(0.0) {}
        explicit Double(double value) : ValueType(value) {}
        int32_t GetHashCode() const override
        {
            if (m_value == 0.0)
                return 0;

            int64_t value = *reinterpret_cast<const int64_t*>(&m_value);
            return static_cast<int32_t>(value)
                 ^ static_cast<int32_t>(value >> 32);
        }

    };
}

#endif  //VWRTK_CWEL_OBJECT_HPP
