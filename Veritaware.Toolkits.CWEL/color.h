#ifndef VERITAWARE_TOOLKITS_CWEL_COLOR_
#define VERITAWARE_TOOLKITS_CWEL_COLOR_

#include <cstdint>

namespace veritaware
{
	struct Color
	{
		Color() = default;
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		Color(float r, float g, float b, float a);
		explicit Color(const uint32_t rgba) { RGBA(rgba); }

		uint8_t R() const { return _r; }
		uint8_t G() const { return _g; }
		uint8_t B() const { return _b; }
		uint8_t A() const { return _a; }

		float Rf() const { return _r / 255.0f; }
		float Gf() const { return _g / 255.0f; }
		float Bf() const { return _b / 255.0f; }
		float Af() const { return _a / 255.0f; }

		uint32_t RGBA() const;

		void R(const uint8_t r) { _r = r; }
		void G(const uint8_t g) { _g = g; }
		void B(const uint8_t b) { _b = b; }
		void A(const uint8_t a) { _a = a; }

		void R(const float r) { _r = static_cast<uint8_t>(Trunc(r) * 255); }
		void G(const float g) { _g = static_cast<uint8_t>(Trunc(g) * 255); }
		void B(const float b) { _b = static_cast<uint8_t>(Trunc(b) * 255); }
		void A(const float a) { _a = static_cast<uint8_t>(Trunc(a) * 255); }

		void Rf(const float r) { R(r); }
		void Gf(const float g) { G(g); }
		void Bf(const float b) { B(b); }
		void Af(const float a) { A(a); }

		void RGBA(uint32_t rgba);

	private:
		uint8_t _r = 0, _g = 0, _b = 0, _a = 0;
		static float Trunc(float f);
	};

	inline Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		_r = r;
		_g = g;
		_b = b;
		_a = a;
	}

	inline Color::Color(const float r, const float g, const float b, const float a)
	{
		_r = static_cast<uint8_t>(Trunc(r) * 255);
		_g = static_cast<uint8_t>(Trunc(g) * 255);
		_b = static_cast<uint8_t>(Trunc(b) * 255);
		_a = static_cast<uint8_t>(Trunc(a) * 255);
	}

	inline uint32_t Color::RGBA() const
	{
		return _r | _g << 8 | _b << 16 | _a << 24;
	}

	inline void Color::RGBA(const uint32_t rgba)
	{
		_r = rgba & 0x000000ff;
		_g = (rgba & 0x0000ff00) >> 8;
		_b = (rgba & 0x00ff0000) >> 16;
		_a = (rgba & 0xff000000) >> 24;
	}

	inline float Color::Trunc(const float f)
	{
		return f > 1.0f ? 1.0f : f < 0.0f ? 0.0f : f;
	}
}

#endif  //VERITAWARE_TOOLKITS_CWEL_COLOR_
