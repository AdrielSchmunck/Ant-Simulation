﻿#ifndef ANT_H
#define ANT_H

#include "raylib-cpp.hpp"
#include "raymath.h"

#include <vector>

#define NEURON_ANGLE 10				//configuracion de neurona de feromonas. Estos valores andan bien
#define NEURON_RANGE 7
#define SENSOR_SIZE 3
#define PHEROMONE_RESPONSE_STRENGTH 2

#define WANDERING_STRENGT 0.006				//que tanto la hormiga se desvia. Valor mas grande = se desvia mas

#define PHEROMONE_FOOD_MIN 10				//minimo nivel de feromona que deja una hormiga
#define PHEROMONE_HOME_MIN 10

#define PHEROMONE_HOME_MAX 4000				//maximo valor de una feromona en un punto
#define PHEROMONE_FOOD_MAX 4000

#define PHEROMONE_FOOD_MULTIPLICATOR 1	//escala el nivel de feromona que una hormiga sube en el campo de feromonas
#define PHEROMONE_HOME_MULTIPLICATOR 1

#define PHEROMONE_HOME_RECHARGE 300			//recarga de feromona cuando una hormiga encuentra comida o casa
#define PHEROMONE_FOOD_RECHARGE 400




typedef struct {
	int home;
	int food;
}pheromones;

class ant
{
public:

	float	wanderStrength= WANDERING_STRENGT;

	raylib::Vector2 position;
	raylib::Vector2 velocity;
	bool hasFood=false;
	int homePheromonePower=PHEROMONE_HOME_RECHARGE;
	int foodPheromonePower=0;


	void update(float timeStep, Image& map, std::vector<pheromones>& pheromoneMap);
	int checkColision(Image& image, float timeStep);
	unsigned int getScoreArroundPoint(std::vector<pheromones>& pheromoneMap, raylib::Vector2 checkPosition, bool pheromoneType);

};

#endif