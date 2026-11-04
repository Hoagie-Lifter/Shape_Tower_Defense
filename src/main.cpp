#include <iostream>
#include "vec2.hpp"
#include <SFML/Graphics.hpp>

int main()
{
	Vec2 v1(100, 100);
	Vec2 v2(200, 200);
	Vec2 v3 = v1 + v2;
	
	std::cout << v3.x << " " << v3.y << "\n";
	
	return 0;
}