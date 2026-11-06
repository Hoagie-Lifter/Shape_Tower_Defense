#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

template <typename T>
class Vec2
{
public:
	// Vec2f or Vec2<float> will make these floats, Vec2 or Vec2<int> will make these ints, etc.
	T x = 0;
	T y = 0;
	
	// default constructor, will be (0, 0)
	Vec2() = default;
	
	Vec2(T xin, T yin)
		: x(xin), y(yin)
	{}
	
	// convert between Vec2 and sf::Vector2 types
	template <typename U>
	Vec2(const sf::Vector2<U> & vec)
		: x(T(vec.x))
		, y(T(vec.y))
	{}
	
	// allow automatic conversion to sf::Vector2
	// allows passing Vec2 into sfml functions
	operator sf::Vector2<T>() const
	{
		return sf::Vector2<T>(x, y);
	}
	
	Vec2 operator + (const Vec2 & rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	
	Vec2 operator - (const Vec2 & rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	
	Vec2 operator * (const T val) const
	{
		return Vec2(x * val, y * val);
	}
	
	Vec2 operator / (const T val) const
	{
		return Vec2(x / val, y / val);
	}
	
	void operator += (const Vec2 & rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	
	void operator -= (const Vec2 & rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	
	void operator *= (const T val)
	{
		x *= val;
		y *= val;
	}
	
	void operator /= (const T val)
	{
		x /= val;
		y /= val;
	}
	
	bool operator == (const Vec2 & rhs) const
	{
		// only true if x AND y are equal to rhs
		return (x == rhs.x && y == rhs.y);
	}
	
	bool operator != (const Vec2 & rhs) const
	{
		// only true if x OR y are not equal to rhs
		return (x != rhs.x || y != rhs.y);
	}
	
	float length() const
	{
		// pythagorean theorem 
		return std::sqrt(x * x + y * y);
	}
	
	float distance(const Vec2 & rhs) const
	{
		return Vec2(rhs.x - x, rhs.y - y).length();
	}
	
	void normalize()
	{
		float l = length();
		x /= l;
		y /= l;
	}
	
};

using Vec2f = Vec2<float>;