#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib-cpp.hpp"
#include "raymath.h"



class obstacle
{
public:

	obstacle(Vector2 placePosition);

	Vector2 position;
	int radius = 20;
	raylib::Color color = GRAY;

};

#endif