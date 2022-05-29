
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "ant.h"
#include "obstacle.h"
#include <vector>


void ant::update(float timeStep, std::vector<obstacle> obstacles) 
{
	//randomDesire = { (float)(rand() % 100) , (float)(rand() % 100) };
	//desiredDirection = (desiredDirection + randomDesire).Normalize();

	velocityDesviation = { (float)(rand() % 100 - 50) , (float)(rand() % 100 - 50) };

	velocity = (velocity + velocityDesviation * wanderStrength).Normalize() * 2;
	position = position + velocity * (float)(checkColision(obstacles, timeStep) * timeStep);	//puede haber error de orden aca

}

int ant::checkColision(std::vector<obstacle> obstacles, float timeStep) 
{
	for (auto &i : obstacles)
	{
		if (CheckCollisionCircles(i.position, i.radius, (position + velocity * timeStep), 5))
			return -1;		
	}
	return 1;
}

