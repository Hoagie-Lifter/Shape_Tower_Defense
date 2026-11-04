#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>

template <typename T>
class Vec2
{
public:
	// Vec2f or Vec2<float> will make these floats, Vec2 or Vec2<int> will make these ints, etc.
	T x = 0;
	T y = 0;
	
	// default constructor, will be (0, 0)
	Vec2() = default;
	
	Vec2(T xin, t yin)
		: x(xin), y(yin)
	{}
	
	// convert between Vec2 and sf::Vector2 types
	template <typename U>
	Vec2(const sf::Vector2<U> & vec)
		: x(T(vec.x))
		, y(T(vec.y))
	{}
};

using Vec2f = Vec2<float>;