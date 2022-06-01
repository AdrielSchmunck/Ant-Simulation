
#include "raylib-cpp.hpp"
#include "raymath.h"
#include "ant.h"
#include <iostream>
#include <vector>

using namespace std;


void ant::update(float timeStep, Image &map, std::vector<pheromones> &pheromoneMap)
{
	raylib::Vector2 velocityDesviation;
	raylib::Vector2 pheromoneVelocity;
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
	else		//no detecte nada de terreno o comida
	{			
		//checkeo feromonas
		if (hasFood)		//NOTA: que sea position - ..... es empirico, con + (que es lo que tiene sentido) no anda
		{
			neurone[0] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, -NEURON_ANGLE) * NEURON_RANGE, 0);
			neurone[1] = getScoreArroundPoint(pheromoneMap, position + velocity * NEURON_RANGE, 0);
			neurone[2] = getScoreArroundPoint(pheromoneMap, position - (raylib::Vector2)Vector2Rotate(velocity, 0) * NEURON_RANGE, 0);
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

		//dejo feromona si no me choque con nada
		if (1)
		{
			if (hasFood)
			{
				if (pheromoneMap[(((int)position.y) * map.width + (int)position.x)].food < 5000)
				{
					pheromoneMap[((int)position.y * map.width + (int)position.x)].food += (int)(foodPheromonePower * 0.5);
					if (foodPheromonePower > 20)
						foodPheromonePower -= 1;

					//pheromoneMap[((int)position.y * map.width + (int)position.x)].food += 20;
				}

			}
			else
				if (pheromoneMap[((int)position.y * map.width + (int)position.x)].home < 5000)
				{
					pheromoneMap[((int)position.y * map.width + (int)position.x)].home += (int)(homePheromonePower * 0.5);
					if (homePheromonePower > 10)
						homePheromonePower -= 1;
					//pheromoneMap[((int)position.y * map.width + (int)position.x)].home += 20;

				}
		}
		
	}

	position = position + velocity * timeStep;	//actualizo la posicion

}

int ant::checkColision(Image& map, float timeStep)
{
	raylib::Vector2 futurePositionFoward = position + velocity * timeStep;
	raylib::Vector2 morefuturePositionFoward = position + velocity * timeStep*2;
	//raylib::Vector2 futurePositionLeft = position - Vector2Rotate(velocity * timeStep,-NEURON_ANGLE) ;
	//raylib::Vector2 futurePositionRight = position - Vector2Rotate(velocity * timeStep, NEURON_ANGLE);


	raylib::Color pixelFoward = GetImageColor(map, (int)futurePositionFoward.x, (int)futurePositionFoward.y);
	raylib::Color pixelMoreFoward = GetImageColor(map, (int)morefuturePositionFoward.x, (int)morefuturePositionFoward.y);
	//raylib::Color pixelLeft = GetImageColor(map, (int)futurePositionLeft.x, (int)futurePositionLeft.y);
	//raylib::Color pixelRight = GetImageColor(map, (int)futurePositionRight.x, (int)futurePositionRight.y);

	raylib::Color pixelWall = BLACK;
	raylib::Color pixelFood = BLUE;
	raylib::Color pixelHome = YELLOW;

	if ((pixelMoreFoward==pixelWall) || (pixelFoward == pixelWall) || (pixelFoward == pixelFood) || (pixelFoward == pixelHome))
	{
		if (pixelFoward == pixelFood)		
		{
			if (!hasFood)			//si detecto comida y no tengo comida, como. Devuelvo 4. Si no, 5
			{
				hasFood = 1;
				foodPheromonePower = 300;
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
				homePheromonePower = 300;
				return 7;
			}
		}
		else
			return 1;		//detecto pared o comida o casa 
							//PUEDO AGREGAR: Que gire al lado opuesto si ve pared de un lado
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
	return (newScore > 0 ? newScore : 0);
	return (pheromoneType ? scoreFood : scoreHome);
}

