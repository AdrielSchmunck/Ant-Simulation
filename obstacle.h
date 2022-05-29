#include "raylib-cpp.hpp"
#include "raymath.h"

class obstacle
{
public:

	obstacle(Vector2 placePosition);

	Vector2 position;
	int radius = 10;
	raylib::Color color = GRAY;

};