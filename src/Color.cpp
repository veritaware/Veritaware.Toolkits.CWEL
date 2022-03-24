#include <sstream>
#include "Color.h"

namespace vwr
{
	Color::Color(const Color & c)
	{
        _a = c._a;
		_r = c._r;
		_g = c._g;
		_b = c._b;
	}

	Color::Color(const uint8_t a, const uint8_t r, const uint8_t g, const uint8_t b)
	{
        _a = a;
		_r = r;
		_g = g;
		_b = b;
	}

	Color::Color(const float a, const float r, const float g, const float b)
	{
        _a = static_cast<uint8_t>(Trunc(a) * 255);
		_r = static_cast<uint8_t>(Trunc(r) * 255);
		_g = static_cast<uint8_t>(Trunc(g) * 255);
		_b = static_cast<uint8_t>(Trunc(b) * 255);
	}

	Color::Color(const uint32_t value, const uint8_t byteOrder)
	{
		switch (byteOrder)
		{
		case BYTE_ORDER_RGBA:
			RGBA(value);
			break;
		case BYTE_ORDER_BGRA:
			BGRA(value);
			break;
		case BYTE_ORDER_ABGR:
			ABGR(value);
			break;
		case BYTE_ORDER_ARGB:
		default:
			ARGB(value);
			break;
		}
	}

    int32_t Color::GetHashCode() const {
        auto hashCode = vwr::GetHashCode(GetType().hash_code());
        hashCode = CombineHashCodes(hashCode, vwr::GetHashCode(_a));
        hashCode = CombineHashCodes(hashCode, vwr::GetHashCode(_r));
        hashCode = CombineHashCodes(hashCode, vwr::GetHashCode(_g));
        hashCode = CombineHashCodes(hashCode, vwr::GetHashCode(_b));
        return hashCode;
    }

    std::string Color::ToString() const {
        std::stringstream ss;
        ss << "#" << std::hex << ARGB();
        return ss.str();
    }

	uint32_t Color::ARGB() const
	{
		auto r = static_cast<uint32_t>(_r);
		auto g = static_cast<uint32_t>(_g);
		auto b = static_cast<uint32_t>(_b);
		auto a = static_cast<uint32_t>(_a);
		return a << 24 | r << 16 | g << 8 | b;
	}

	uint32_t Color::RGBA() const
	{
		auto r = static_cast<uint32_t>(_r);
		auto g = static_cast<uint32_t>(_g);
		auto b = static_cast<uint32_t>(_b);
		auto a = static_cast<uint32_t>(_a);
		return r << 24 | g << 16 | b << 8 | a ;
	}

	uint32_t Color::ABGR() const
	{
		auto r = static_cast<uint32_t>(_r);
		auto g = static_cast<uint32_t>(_g);
		auto b = static_cast<uint32_t>(_b);
		auto a = static_cast<uint32_t>(_a);
		return a << 24 | b << 16 | g << 8 | r;
	}

	uint32_t Color::BGRA() const
	{
		auto r = static_cast<uint32_t>(_r);
		auto g = static_cast<uint32_t>(_g);
		auto b = static_cast<uint32_t>(_b);
		auto a = static_cast<uint32_t>(_a);
		return b << 24 | g << 16 | r << 8 | a;
	}

	void Color::ARGB(const uint32_t argb)
	{
		_a = (argb & 0xff000000) >> 24;
		_r = (argb & 0x00ff0000) >> 16;
		_g = (argb & 0x0000ff00) >> 8;
		_b = argb & 0x000000ff;
	}

	void Color::RGBA(const uint32_t rgba)
	{
		_r = (rgba & 0xff000000) >> 24;
		_g = (rgba & 0x00ff0000) >> 16;
		_b = (rgba & 0x0000ff00) >> 8;
		_a = rgba & 0x000000ff;
	}

	void Color::ABGR(const uint32_t abgr)
	{
		_a = (abgr & 0xff000000) >> 24;
		_b = (abgr & 0x00ff0000) >> 16;
		_g = (abgr & 0x0000ff00) >> 8;
		_r = abgr & 0x000000ff;
	}

	void Color::BGRA(const uint32_t bgra)
	{
		_b = (bgra & 0xff000000) >> 24;
		_g = (bgra & 0x00ff0000) >> 16;
		_r = (bgra & 0x0000ff00) >> 8;
		_a = bgra & 0x000000ff;
	}

	float Color::Trunc(const float f)
	{
		return f > 1.0f ? 1.0f : f < 0.0f ? 0.0f : f;
	}
}