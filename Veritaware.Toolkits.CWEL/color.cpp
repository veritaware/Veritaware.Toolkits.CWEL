#include "stdafx.h"
#include "color.h"

namespace veritaware
{
	Color::Color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
	{
		_r = r;
		_g = g;
		_b = b;
		_a = a;
	}

	Color::Color(const float r, const float g, const float b, const float a)
	{
		_r = static_cast<uint8_t>(Trunc(r) * 255);
		_g = static_cast<uint8_t>(Trunc(g) * 255);
		_b = static_cast<uint8_t>(Trunc(b) * 255);
		_a = static_cast<uint8_t>(Trunc(a) * 255);
	}

	Color::Color(const uint32_t value, const uint8_t byteOrder)
	{
		switch (byteOrder)
		{
		case BYTE_ORDER_RGBA:
			RGBA(value);
			break;
		case BYTE_ORDER_BGRA:
			break;
		case BYTE_ORDER_ABGR:
			break;
		case BYTE_ORDER_ARGB:
		default:
			ARGB(value);
			break;
		}
	}

	uint32_t Color::ARGB() const
	{
		return _a | _r << 8 | _g << 16 | _b << 24;
	}

	uint32_t Color::RGBA() const
	{
		return _r | _g << 8 | _b << 16 | _a << 24;
	}

	uint32_t Color::ABGR() const
	{
		return _a | _b << 8 | _g << 16 | _r << 24;
	}

	uint32_t Color::BGRA() const
	{
		return _b | _g << 8 | _r << 16 | _a << 24;
	}

	void Color::ARGB(const uint32_t argb)
	{
		_a = argb & 0x000000ff;
		_r = (argb & 0x0000ff00) >> 8;
		_g = (argb & 0x00ff0000) >> 16;
		_b = (argb & 0xff000000) >> 24;
	}

	void Color::RGBA(const uint32_t rgba)
	{
		_r = rgba & 0x000000ff;
		_g = (rgba & 0x0000ff00) >> 8;
		_b = (rgba & 0x00ff0000) >> 16;
		_a = (rgba & 0xff000000) >> 24;
	}

	void Color::ABGR(const uint32_t abgr)
	{
		_a = abgr & 0x000000ff;
		_b = (abgr & 0x0000ff00) >> 8;
		_g = (abgr & 0x00ff0000) >> 16;
		_r = (abgr & 0xff000000) >> 24;
	}

	void Color::BGRA(const uint32_t bgra)
	{
		_b = bgra & 0x000000ff;
		_g = (bgra & 0x0000ff00) >> 8;
		_r = (bgra & 0x00ff0000) >> 16;
		_a = (bgra & 0xff000000) >> 24;
	}

	float Color::Trunc(const float f)
	{
		return f > 1.0f ? 1.0f : f < 0.0f ? 0.0f : f;
	}
}