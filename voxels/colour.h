#ifndef colour_h
#define colour_h

struct Colour {
	union {
		struct {float r, g, b, a;};
		float raw[4];
	};
	
	Colour(): r(0), g(0), b(0), a(1) {}
	Colour(float _r, float _g, float _b) : r(_r), g(_g), b(_b), a(1) {}
	Colour(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
	
	Colour(int c) {
		r = ((c & 0xFF0000) >> 16) / 255.f;
		g = ((c & 0xFF00) >> 8) / 255.f;
		b = (c & 0xFF) / 255.f;
	}
	
	inline Colour operator +(const Colour &v) const {
		return Colour(r + v.r, g + v.g, b + v.b, a + v.a);
	}
	
	inline Colour operator -(const Colour &v) const {
		return Colour(r - v.r, g - v.g, b - v.b, a - v.a);
	}
	
	inline Colour operator *(float s) const {
		return Colour(r * s, g * s, b * s, a * s);
	}
	
	inline Colour operator /(float s) const {
		return Colour(r / s, g / s, b / s, a / s);
	}
	
	inline Colour operator +=(const Colour &v) {
		r += v.r;
		g += v.g;
		b += v.b;
		a += v.a;
		return *this;
	}
	
	inline Colour operator -=(const Colour &v) {
		r += v.r;
		g += v.g;
		b += v.b;
		a -= v.a;
		return *this;
	}
	
	inline Colour operator *=(float s) {
		r *= s;
		g *= s;
		b *= s;
		a *= s;
		return *this;
	}
	
	inline Colour operator /=(float s) {
		r /= s;
		g /= s;
		b /= s;
		a /= s;
		return *this;
	}
};

#endif
