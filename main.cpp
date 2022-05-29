/*
 * Orbital simulation
 *
 * 22.08 EDA
 * Copyright (C) 2022 Marc S. Ressl
 *
 * Main module
 */

#include <stdlib.h>
#include <stdio.h>
#include "iostream"
#include "ant.h"
#include "obstacle.h"
#include <vector>


#include "raylib-cpp.hpp"


#define TIME_STEP 0.01f

using namespace std;

int main()
{

    ant hormiga;

    ant ant[100];


    for (auto& i : ant)
    {
        i.position = { 0,0 };
    }
        
    vector<obstacle> obstacles;
    
        
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Ant Simulation");

    Camera2D camera;
    //camera.target = { screenWidth/2, screenHeight/2};
    camera.target = { 0, 0 };
    camera.offset = { screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    
    while (!WindowShouldClose())
    {
        

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            obstacles.push_back(obstacle(Vector2Subtract(GetMousePosition() , camera.offset)));
        }


        for (auto& i : ant)
        {
            i.update(TIME_STEP, obstacles);
        }

       
        // Render
        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(camera);

        for (auto& i : ant)
        {
            DrawPixel(i.position.x, i.position.y, BLACK);
        }
       
        for (auto& a : obstacles)
        {
            DrawCircle(a.position.x, a.position.y, a.radius, GRAY);
        }
             

        EndMode2D();

        //camera.BeginMode();
        //simview.renderOrbitalSim3D(&sim);

        //DrawGrid(10, 10.0f);
        //camera.EndMode();

        //simview.renderOrbitalSim2D(&sim);
        EndDrawing();

        cout << obstacles.size() << endl;
    }

    CloseWindow();


    return 0;
}


