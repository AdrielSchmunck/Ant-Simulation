
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "ant.h"
#include <iostream>
#include <vector>

using namespace std;


void ant::update(float timeStep, Image &map, std::vector<pheromones> &pheromoneMap)
{
	raylib::Vector2 pheromoneVelocity;
	vector<float> neurone(3);

	velocityDesviation = { (float)(rand() % 101 - 50) , (float)(rand() % 101 - 50) };
	
	if (hasFood)		//NOTA: que sea position - ..... es empirico, con + (que es lo que tiene sentido) no anda
	{
		neurone[0] = getScoreArroundPoint(pheromoneMap,position - (raylib::Vector2)Vector2Rotate(velocity,-NEURON_ANGLE)*NEURON_RANGE, 0);
		neurone[1] = getScoreArroundPoint(pheromoneMap, position - velocity * NEURON_RANGE, 0);
		neurone[2] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, 0) * NEURON_RANGE, 0);	
	}
	else
	{
		neurone[0] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE) * NEURON_RANGE, 1);
		neurone[1] = getScoreArroundPoint(pheromoneMap, position - velocity * NEURON_RANGE, 1);
		neurone[2] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE) * NEURON_RANGE, 1);
	}
	
	
	if((neurone[1]>neurone[0])&&(neurone[1]>neurone[2]))
		pheromoneVelocity = 0;
	else if ((neurone[1] < neurone[0]) && (neurone[1] < neurone[2]))
	{
		if(neurone[0]>neurone[2])
			pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE);
		else
			pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE);
	}
	else if(neurone[0]>neurone[2])
		pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE);
	else if(neurone[0]<neurone[2])
		pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE) ;
	else
		pheromoneVelocity = 0;


	/*max = neurone[0] > neurone[1] ? 0 : 1;
	max = neurone[max] > neurone[2] ? max : 2;


	if (max == 0)
		pheromoneVelocity =  -(raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE) * neurone[0];
	else if (max == 1)
		pheromoneVelocity = -velocity * neurone[1];
	else if (max==2)
		pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE) * neurone[2];*/


	velocity = (velocity + pheromoneVelocity*10 + velocityDesviation * wanderStrength).Normalize();
	velocity = velocity * checkColision(map, timeStep);	
	position = position + velocity * timeStep;

	if(checkColision(map, timeStep)==1)
	{
		if (hasFood)
		{
			if (pheromoneMap[(int)(position.y * map.width + position.x)].food < 10000)
			{
				pheromoneMap[((int)position.y * map.width + (int)position.x)].food += 1;

			}

		}
		else
			if (pheromoneMap[(int)(position.y * map.width + position.x)].home < 10000)
			{
				pheromoneMap[((int)position.y * map.width + (int)position.x)].home += 1;

			}
	}
	
}

int ant::checkColision(Image& image, float timeStep)
{
	raylib::Vector2 futurePosition = position + velocity * timeStep;


	raylib::Color pixel = GetImageColor(image, (int)futurePosition.x, (int)futurePosition.y);
	raylib::Color pixelCheck = BLACK;
	raylib::Color pixelFood = BLUE;
	raylib::Color pixelHome = YELLOW;
	if (pixel == pixelCheck)
	{
		return -1;
	}
	else if (pixel == pixelFood)
	{
		hasFood = true;
		return -1;
	}	
	else if (pixel == pixelHome)
	{
		hasFood = false;
		//return -1;
	}
		

	return 1;

}

unsigned int ant::getScoreArroundPoint(std::vector<pheromones> &pheromoneMap, raylib::Vector2 position, bool pheromoneType)
{
	float score=0;
	if (!pheromoneType)
	{
		for (int x = -3; x <= 3; x++)
		{
			for (int y = -3; y <= 3; y++)
			{
				score += pheromoneMap[(y + (int)position.y) * 1000 + x + (int)position.x].home;
			}
		}
	}
	else
		for (int x = -3; x <= 3; x++)
		{
			for (int y = -3; y <= 3; y++)
			{
				score += pheromoneMap[(y + (int)position.y) * 1000 + x+ (int)position.x].food;
			}
		}
	return score;
}

