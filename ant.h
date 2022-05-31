#ifndef ANT_H
#define ANT_H

#include "raylib-cpp.hpp"
#include "raymath.h"

#include <vector>

#define NEURON_ANGLE 45
#define NEURON_RANGE 10

typedef struct {
	int home;
	int food;
}pheromones;

class ant
{
public:

	float	wanderStrength=0.001;

	raylib::Vector2 position;
	raylib::Vector2 velocity;
	raylib::Vector2 velocityDesviation;
	bool hasFood=false;


	void update(float timeStep, Image& image, std::vector<pheromones>& pheromoneMap);
	int checkColision(Image& image, float timeStep);
	unsigned int getScoreArroundPoint(std::vector<pheromones>& pheromoneMap, raylib::Vector2 position, bool pheromoneType);

};

#endif