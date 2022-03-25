#ifndef VERITAWARE_TOOLKITS_CWEL_COLOR_
#define VERITAWARE_TOOLKITS_CWEL_COLOR_

#include <cmath>
#include <cstdint>

#include "Object.hpp"

namespace vwr
{
#define BYTE_ORDER_ARGB 0x00
#define BYTE_ORDER_RGBA 0x01
#define BYTE_ORDER_ABGR 0x10
#define BYTE_ORDER_BGRA 0x11

	class Color : public Object
	{
	public:
		Color() = default;
		Color(const Color & c);
		Color(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
		Color(float a, float r, float g, float b);
		explicit Color(uint32_t value, uint8_t byteOrder = BYTE_ORDER_ARGB);

        int32_t GetHashCode() const override;
        std::string ToString() const override;

        uint8_t A() const { return _a; }
		uint8_t R() const { return _r; }
		uint8_t G() const { return _g; }
		uint8_t B() const { return _b; }

        float Af() const { return _a / 255.0f; }
		float Rf() const { return _r / 255.0f; }
		float Gf() const { return _g / 255.0f; }
		float Bf() const { return _b / 255.0f; }

		uint32_t ARGB() const;
		uint32_t RGBA() const;
		uint32_t ABGR() const;
		uint32_t BGRA() const;

        void A(const uint8_t a) { _a = a; }
		void R(const uint8_t r) { _r = r; }
		void G(const uint8_t g) { _g = g; }
		void B(const uint8_t b) { _b = b; }

        void A(const float a) { _a = static_cast<uint8_t>(round(a * 255.0f)); }
		void R(const float r) { _r = static_cast<uint8_t>(round(r * 255.0f)); }
		void G(const float g) { _g = static_cast<uint8_t>(round(g * 255.0f)); }
		void B(const float b) { _b = static_cast<uint8_t>(round(b * 255.0f)); }

        void Af(const float a) { A(a); }
		void Rf(const float r) { R(r); }
		void Gf(const float g) { G(g); }
		void Bf(const float b) { B(b); }

		void ARGB(uint32_t argb);
		void RGBA(uint32_t rgba);
		void ABGR(uint32_t abgr);
		void BGRA(uint32_t bgra);

	private:
		uint8_t _a = 0, _r = 0, _g = 0, _b = 0;
		static float Trunc(float f);
	};
}

#endif  //VERITAWARE_TOOLKITS_CWEL_COLOR_
