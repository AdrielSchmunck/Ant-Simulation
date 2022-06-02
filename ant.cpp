
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "ant.h"
#include <iostream>
#include <vector>

using namespace std;


void ant::update(float timeStep, Image &map, std::vector<pheromones> &pheromoneMap)
{
	raylib::Vector2 velocityDesviation;
	raylib::Vector2 pheromoneVelocity = Vector2Zero();
	vector<unsigned int> neurone(3);
	
	neurone[0] = 0;
	neurone[1] = 0;
	neurone[2] = 0;

	velocityDesviation = { (float)(rand() % 101 - 50) , (float)(rand() % 101 - 50) };
	
	int colision = checkColision(map, timeStep);

	//cout << colision << endl;

	if (colision==1)				//si detecte pared, velocidad=0 y salgo
	{
		//velocity = Vector2Zero();
		velocity = velocityDesviation.Normalize();
		return;		
	}
	else if (colision == 4)
	{
		velocity = -velocity;
	}
	else if (colision == 5)
	{
		velocity = (velocity + velocityDesviation * wanderStrength).Normalize();
	}
	else if (colision == 6)
	{
		velocity = (velocity + velocityDesviation * wanderStrength).Normalize();
	}
	else if (colision == 7)
	{
		velocity = -velocity;
	}
	else if (colision == 8)
	{
		velocity = velocity;
	}
	else if (colision == 9)
	{
		velocity = velocity;
	}
	else		//no detecte nada de terreno o comida
	{			
		//checkeo feromonas
		if (hasFood)		//NOTA: que sea position - ..... es empirico, con + (que es lo que tiene sentido) no anda
		{
			neurone[0] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE) * NEURON_RANGE, 0);
			neurone[1] = getScoreArroundPoint(pheromoneMap, position + velocity * NEURON_RANGE, 0);
			neurone[2] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE) * NEURON_RANGE, 0);
		}
		else
		{
			neurone[0] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE) * NEURON_RANGE, 1);
			neurone[1] = getScoreArroundPoint(pheromoneMap, position + velocity * NEURON_RANGE, 1);
			neurone[2] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE) * NEURON_RANGE, 1);
		}

		if ((neurone[1] > neurone[0]) && (neurone[1] > neurone[2]))
			pheromoneVelocity = { 0,0 };
		else if ((neurone[1] < neurone[0]) && (neurone[1] < neurone[2]))
		{
			if (neurone[0] > neurone[2])
				pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE);
			else
				pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE);
		}
		else if (neurone[0] > neurone[2])
			pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE);
		else if (neurone[0] < neurone[2])
			pheromoneVelocity = -(raylib::Vector2)Vector2Rotate(velocity, NEURON_ANGLE);
		else
			pheromoneVelocity = { 0,0 };

		//actualizo velocidad. Si tengo una pheromoneVelocity, es lo que mas influye la velocidad final

		velocity = (velocity + pheromoneVelocity*PHEROMONE_RESPONSE_STRENGTH +velocityDesviation * wanderStrength).Normalize();

		//dejo feromona
		/*if (hasFood)
		{
			if (pheromoneMap[(((int)position.y) * map.width + (int)position.x)].food < PHEROMONE_FOOD_MAX)
			{
				
				pheromoneMap[((int)position.y * map.width + (int)position.x)].food += (foodPheromonePower * PHEROMONE_FOOD_MULTIPLICATOR);
				if (foodPheromonePower > PHEROMONE_FOOD_MIN)
				{
					foodPheromonePower -= 1;			
				}
			}
		}
		else
			if (pheromoneMap[((int)position.y * map.width + (int)position.x)].home < PHEROMONE_HOME_MAX)
			{
				
				pheromoneMap[((int)position.y * map.width + (int)position.x)].home += (homePheromonePower * PHEROMONE_HOME_MULTIPLICATOR);
				if (homePheromonePower > PHEROMONE_HOME_MIN)
				{
					homePheromonePower -= 1;
				}					
			}*/
		}
		
	if (hasFood)
	{
		if (pheromoneMap[(((int)position.y) * map.width + (int)position.x)].food < PHEROMONE_FOOD_MAX)
		{

			pheromoneMap[((int)position.y * map.width + (int)position.x)].food += (foodPheromonePower * PHEROMONE_FOOD_MULTIPLICATOR);
			if (foodPheromonePower > PHEROMONE_FOOD_MIN)
			{
				foodPheromonePower -= 1;
			}
		}
	}
	else
		if (pheromoneMap[((int)position.y * map.width + (int)position.x)].home < PHEROMONE_HOME_MAX)
		{

			pheromoneMap[((int)position.y * map.width + (int)position.x)].home += (homePheromonePower * PHEROMONE_HOME_MULTIPLICATOR);
			if (homePheromonePower > PHEROMONE_HOME_MIN)
			{
				homePheromonePower -= 1;
			}
		}

	position = position + velocity * timeStep;	//actualizo la posicion
}

int ant::checkColision(Image& map, float timeStep)
{
	raylib::Vector2 futurePositionFoward = position + velocity * timeStep;
	raylib::Vector2 morefuturePositionFoward = position + velocity * timeStep*20;
	raylib::Vector2 littlefuturePositionFoward = position + velocity * timeStep * 5;
	//raylib::Vector2 futurePositionLeft = position - Vector2Rotate(velocity * timeStep,-NEURON_ANGLE) ;
	//raylib::Vector2 futurePositionRight = position - Vector2Rotate(velocity * timeStep, NEURON_ANGLE);


	raylib::Color pixelFoward = GetImageColor(map, (int)futurePositionFoward.x, (int)futurePositionFoward.y);
	raylib::Color pixelMoreFoward = GetImageColor(map, (int)morefuturePositionFoward.x, (int)morefuturePositionFoward.y);
	raylib::Color pixelLittleFoward = GetImageColor(map, (int)littlefuturePositionFoward.x, (int)littlefuturePositionFoward.y);
	//raylib::Color pixelLeft = GetImageColor(map, (int)futurePositionLeft.x, (int)futurePositionLeft.y);
	//raylib::Color pixelRight = GetImageColor(map, (int)futurePositionRight.x, (int)futurePositionRight.y);

	raylib::Color pixelWall = BLACK;
	raylib::Color pixelFood = BLUE;
	raylib::Color pixelHome = YELLOW;

	if ((pixelLittleFoward == pixelWall)||(pixelFoward == pixelWall) || (pixelFoward == pixelFood) || (pixelFoward == pixelHome))
	{
		if (pixelFoward == pixelFood)		
		{
			if (!hasFood)			//si detecto comida y no tengo comida, como. Devuelvo 4. Si no, 5
			{
				hasFood = 1;
				foodPheromonePower = PHEROMONE_FOOD_RECHARGE;
				homePheromonePower = 0;
				return 4;
			}
			else
			{
				return 5;
			}	
		}			
		else if (pixelFoward == pixelHome)	//si detecto casa dejo comida
		{
			if (!hasFood)			//si detecto casa y tengo comida, dejo. Devuelvo 6. Si no, 7
			{
				
				return 6;
			}
			else
			{
				hasFood = 0;
				foodPheromonePower = 0;
				homePheromonePower = PHEROMONE_HOME_RECHARGE;
				return 7;
			}
		}
		else
		{
			/*foodPheromonePower = 0;
			homePheromonePower = 0;*/
			return 1;										//detecto pared 
							
		}
			
	}
	else if ((pixelMoreFoward == pixelFood)&&!hasFood)	//comida bastante mas adelante
	{
		return 8;
	}
	else if ((pixelMoreFoward == pixelHome)&&hasFood)	//casa bastante mas adelante
	{
		return 9;
	}
	
	return 3;
	//else if ((pixelLeft == pixelFood)&&(pixelLeft == pixelHome))
	//{
	//	if (pixelLeft == pixelFood)
	//		hasFood = 1;
	//	else if (pixelLeft == pixelHome)
	//		hasFood = 0;

	//	return(0);			//detecto algo a izquierda
	//}	
	//else if ((pixelRight == pixelFood) && (pixelRight == pixelHome))
	//{		
	//	if (pixelRight == pixelFood)
	//		hasFood = 1;
	//	else if (pixelRight == pixelHome)
	//		hasFood = 0;

	//	return(2);			//detecto algo a la derecha
	//}
	//else
	//{
	//	return(3);			//no detecto nada
	//}

}

unsigned int ant::getScoreArroundPoint(std::vector<pheromones> &pheromoneMap, raylib::Vector2 checkPosition, bool pheromoneType)
{
	int scoreFood=0;
	int scoreHome = 0;
	int newScore = 0;

	for (int x = -SENSOR_SIZE; x <= SENSOR_SIZE; x++)
	{
		for (int y = -SENSOR_SIZE; y <= SENSOR_SIZE; y++)
		{

			/*BeginDrawing();
			DrawCircle(x + (int)checkPosition.x, (y + (int)checkPosition.y), 1, DARKBLUE);
			EndDrawing();*/

			//if(((y + (int)checkPosition.y) * 1000 + x + (int)checkPosition.x) < 1000*600)	//HARDCODEADO
				scoreHome += pheromoneMap[(y + (int)checkPosition.y) * 1000 + x + (int)checkPosition.x].home;

		}
	}
		for (int x = -SENSOR_SIZE; x <= SENSOR_SIZE; x++)
	{
		for (int y = -SENSOR_SIZE; y <= SENSOR_SIZE; y++)
		{
			/*BeginDrawing();
			DrawCircle(x + (int)checkPosition.x, (y + (int)checkPosition.y), 1, DARKBLUE);
			EndDrawing();*/

			//if (((y + (int)checkPosition.y) * 1000 + x + (int)checkPosition.x) < 1000 * 600)	//HARDCODEADO
				scoreFood += pheromoneMap[(y + (int)checkPosition.y) * 1000 + x+ (int)checkPosition.x].food;
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
	//newScore = (newScore > 0 ? newScore : 0);
	//return newScore;
	return (pheromoneType == 1 ? scoreFood : scoreHome);
}

