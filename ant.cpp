
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "ant.h"
#include <iostream>
#include <vector>

using namespace std;


void ant::update(float timeStep, Image &map, std::vector<pheromones> &pheromoneMap)
{
	raylib::Vector2 pheromoneVelocity;
	vector<unsigned int> neurone(3);
	
	neurone[0] = 0;
	neurone[1] = 0;
	neurone[2] = 0;

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
	
	/*if (neurone[1]>0 || neurone[0]>0 || neurone[2]>0)
	{
		if (hasFood&&(foodPheromonePower<100))
			foodPheromonePower = 10;
		else if (!hasFood&&(homePheromonePower<100))
			homePheromonePower = 10;
	}*/
	
	if((neurone[1]>neurone[0])&&(neurone[1]>neurone[2]))
		pheromoneVelocity = { 0,0 };
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
		pheromoneVelocity = {0,0};


	if (pheromoneVelocity == Vector2Zero())
		velocity = (velocity + velocityDesviation * wanderStrength).Normalize();
	else
		velocity = (velocity + pheromoneVelocity * 10 ).Normalize();

	int colision = checkColision(map, timeStep);

	velocity = velocity * colision;	
	position = position + velocity * timeStep;

	if((checkColision(map, timeStep) >= 0) && ((((int)position.y) * map.width + (int)position.x) < 1000 * 600))
	{
		if (hasFood)
		{
			if (pheromoneMap[(((int)position.y) * map.width + (int)position.x)].food < 1000)
			{
				/*pheromoneMap[((int)position.y * map.width + (int)position.x)].food += (int)(foodPheromonePower*0.5);
				if (foodPheromonePower > 50)
					foodPheromonePower -= 1;*/

				pheromoneMap[((int)position.y * map.width + (int)position.x)].food += 10;
			}

		}
		else
			if (pheromoneMap[((int)position.y * map.width + (int)position.x)].home < 1000)
			{
				/*pheromoneMap[((int)position.y * map.width + (int)position.x)].home += (int)(homePheromonePower*0.5);
				if(homePheromonePower > 2)
					homePheromonePower -= 1;*/
				pheromoneMap[((int)position.y * map.width + (int)position.x)].home += 10;

			}
	}
	
}

int ant::checkColision(Image& map, float timeStep)
{
	raylib::Vector2 futurePosition = position + velocity * timeStep;


	raylib::Color pixel = GetImageColor(map, (int)futurePosition.x, (int)futurePosition.y);
	raylib::Color pixelCheck = BLACK;
	raylib::Color pixelFood = BLUE;
	raylib::Color pixelHome = YELLOW;
	if (pixel == pixelCheck)
	{
		return 0;
	}
	else if ((pixel == pixelFood)&&(hasFood==0))
	{
		hasFood = true;
		//homePheromonePower = 0;
		//foodPheromonePower = 300;
		return -1;
	}	
	else if ((pixel == pixelHome)&&(hasFood==1))
	{
		hasFood = false;
		//homePheromonePower = 200;
		//foodPheromonePower = 0;
		return -1;
	}
	else
	{
		return 1;
	}

}

unsigned int ant::getScoreArroundPoint(std::vector<pheromones> &pheromoneMap, raylib::Vector2 checkPosition, bool pheromoneType)
{
	int scoreFood=0;
	int scoreHome = 0;
	int newScore = 0;
	//if (!pheromoneType)
	//{
	//	for (int x = -3; x <= 3; x++)
	//	{
	//		for (int y = -3; y <= 3; y++)
	//		{
	//			if(((y + (int)position.y) * 1000 + x + (int)position.x) < 1000*600)	//HARDCODEADO
	//				score += pheromoneMap[(y + (int)position.y) * 1000 + x + (int)position.x].home;
	//			else
	//				cout << (y + (int)position.y) * 1000 + x + (int)position.x << endl;
	//		}
	//	}
	//}
	//else
	//	for (int x = -3; x <= 3; x++)
	//	{
	//		for (int y = -3; y <= 3; y++)
	//		{
	//			if (((y + (int)position.y) * 1000 + x + (int)position.x) < 1000 * 600)	//HARDCODEADO
	//				score += pheromoneMap[(y + (int)position.y) * 1000 + x+ (int)position.x].food;
	//			else
	//				cout << (y + (int)position.y) * 1000 + x + (int)position.x << endl;
	//		}
	//	}

	for (int x = -SENSOR_SIZE; x <= SENSOR_SIZE; x++)
	{
		for (int y = -SENSOR_SIZE; y <= SENSOR_SIZE; y++)
		{
			if(((y + (int)position.y) * 1000 + x + (int)position.x) < 1000*600)	//HARDCODEADO
				scoreHome += pheromoneMap[(y + (int)position.y) * 1000 + x + (int)position.x].home;
			else
				cout << (y + (int)position.y) * 1000 + x + (int)position.x << endl;
		}
	}
		for (int x = -SENSOR_SIZE; x <= SENSOR_SIZE; x++)
	{
		for (int y = -SENSOR_SIZE; y <= SENSOR_SIZE; y++)
		{
			if (((y + (int)position.y) * 1000 + x + (int)position.x) < 1000 * 600)	//HARDCODEADO
				scoreFood += pheromoneMap[(y + (int)position.y) * 1000 + x+ (int)position.x].food;
			else
				cout << (y + (int)position.y) * 1000 + x + (int)position.x << endl;
		}
	}

	scoreHome = pheromoneMap[((int)checkPosition.y) * 1000 + (int)checkPosition.x].home;
	scoreFood = pheromoneMap[((int)checkPosition.y) * 1000 + (int)checkPosition.x].food;
	if (!pheromoneType)
	{
		newScore = scoreHome - scoreFood;
	}
	else
	{
		newScore = scoreFood - scoreHome;
	}
	//return score;
	//return (newScore > 0 ? newScore : 0);
	return (pheromoneType ? scoreFood : scoreHome);
}

