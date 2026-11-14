#pragma once

#include "vec2.hpp"
#include <SFML/Graphics.hpp>

// all components have default constructors to enable saving in a tuple in entity.hpp

// Component base class exists so that all components can inherit 'exists' boolean
class Component
{
public:
	bool exists = false;
};

class CCircle : public Component
{
public:
	sf::CircleShape circle;
	
	CCircle() = default;
	CCircle(float radius, size_t points, const sf::Color & fill, const sf::Color & outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin({radius, radius}); // sets origin to center of shape
	}
};

class CCollision : public Component
{
public:
	float radius = 0;
	
	CCollision() = default;
	CCollision(float r)
		: radius(r) {}
};

class CInput : public Component
{
public:
	bool up     = false;
	bool down   = false;
	bool left   = false;
	bool right  = false;
	bool shoot  = false;
	
	CInput() = default;
};

class CLifespan : public Component
{
public:
	int lifespan  = 0;
	int remaining = 0;
	
	CLifespan() = default;
	CLifespan(int totalLifespan)
		: lifespan(totalLifespan), remaining(totalLifespan) {}
};

class CRectangle : public Component
{
public:
	sf::RectangleShape rectangle;
	
	CRectangle() = default;
	CRectangle(const sf::Color & fill, const sf::Color & outline, float width, float height, float thickness)
		: rectangle({width, height})
	{
		rectangle.setFillColor(fill);
		rectangle.setOutlineColor(outline);
		rectangle.setOutlineThickness(thickness);
		rectangle.setOrigin({width / 2, height / 2}); // sets origin to center of shape
	}
};

class CScore : public Component
{
public:
	int score = 0;
	
	CScore() = default;
	CScore(int s)
		: score(s) {}
};

class CTargeting : public Component
{
public:
	bool displayed = false;

	sf::CircleShape range;
	
	CTargeting() = default;
	CTargeting(float radius, size_t points, const sf::Color & fill)
		: range(radius, points)
	{
		range.setFillColor(fill);
		range.setOrigin({radius, radius}); // sets origin to center of shape
	}
};

class CTransform : public Component
{
public:
	// angle corresponds to rotation
	float angle		= 0;
	Vec2f pos		= {0.0, 0.0};
	Vec2f velocity  = {0.0, 0.0};
	
	CTransform() = default;
	CTransform(const Vec2f & p)
		: pos(p) {}
	CTransform(const Vec2f & p, const Vec2f & v, float a)
		: pos(p), velocity(v), angle(a) {}
};