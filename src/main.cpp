#include <iostream>
#include "vec2.hpp"
#include <SFML/Graphics.hpp>

int main()
{
	Vec2f v1(259, 296);
	Vec2f v2(567, 245);
	Vec2f v3 = v1 + v2;
	
	float dist = v1.distance(v2);
	float length = v2.length();
	v3.normalize();
	
	std::cout << v3.x << " " << v3.y << "\n";
	std::cout << dist << "\n";
	std::cout << length << "\n";
	
	Vec2f a(1, 2);
	Vec2f b(1, 3);
	std::cout << (a != b) << "\n";
	
	return 0;
}