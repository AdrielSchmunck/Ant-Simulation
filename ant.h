﻿#ifndef ANT_H
#define ANT_H

#include "raylib-cpp.hpp"
#include "raymath.h"
#include "obstacle.h"
#include <vector>

class ant
{
public:

	float	wanderStrength=0.001;

	raylib::Vector2 position;
	raylib::Vector2 velocity;
	//raylib::Vector2 desiredDirection;
	raylib::Vector2 velocityDesviation;
	//raylib::Vector2 randomDesire;


	void update(float timeStep,std::vector<obstacle> obstacles);
	int checkColision(std::vector<obstacle> obstacles, float timeStep);

};

#endif