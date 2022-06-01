#ifndef ANT_H
#define ANT_H

#include "raylib-cpp.hpp"
#include "raymath.h"

#include <vector>

#define NEURON_ANGLE 15
#define NEURON_RANGE 8
#define SENSOR_SIZE 3
#define PHEROMONE_RESPONSE_STRENGTH 1

typedef struct {
	int home;
	int food;
}pheromones;

class ant
{
public:

	float	wanderStrength=0.003;

	raylib::Vector2 position;
	raylib::Vector2 velocity;
	bool hasFood=false;
	int homePheromonePower=300;
	int foodPheromonePower=0;


	void update(float timeStep, Image& map, std::vector<pheromones>& pheromoneMap);
	int checkColision(Image& image, float timeStep);
	unsigned int getScoreArroundPoint(std::vector<pheromones>& pheromoneMap, raylib::Vector2 checkPosition, bool pheromoneType);

};

#endif