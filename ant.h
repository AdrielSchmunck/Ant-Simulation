#ifndef ANT_H
#define ANT_H

#include "raylib-cpp.hpp"
#include "raymath.h"

#include <vector>

#define NEURON_ANGLE 30
#define NEURON_RANGE 5
#define SENSOR_SIZE 10

typedef struct {
	int home;
	int food;
}pheromones;

class ant
{
public:

	float	wanderStrength=0.005;

	raylib::Vector2 position;
	raylib::Vector2 velocity;
	raylib::Vector2 velocityDesviation;
	bool hasFood=false;
	unsigned int homePheromonePower=100;
	unsigned int foodPheromonePower=0;


	void update(float timeStep, Image& map, std::vector<pheromones>& pheromoneMap);
	int checkColision(Image& image, float timeStep);
	unsigned int getScoreArroundPoint(std::vector<pheromones>& pheromoneMap, raylib::Vector2 checkPosition, bool pheromoneType);

};

#endif