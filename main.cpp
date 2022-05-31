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
#include <vector>


#include "raylib-cpp.hpp"


#define TIME_STEP 1.0f
#define ANT_AMOUNT 500
#define ANT_INITIAL_POSITION { screenWidth/2, screenHeight/2 }
#define DIFUSION_TIMER 5

using namespace std;




int main()
{          
    const int screenWidth = 1000;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Ant Simulation");

    Camera2D camera;
    //camera.target = { screenWidth/2, screenHeight/2};
    camera.target = { 0, 0 };
    camera.offset = { screenWidth / 2, screenHeight / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    
    srand(time(NULL));

    ant ant[ANT_AMOUNT];         //create ants

    for (auto& i : ant)         
    {
        i.position = ANT_INITIAL_POSITION;
    }

    raylib::Color terrainColor = BLACK;
    Image map = LoadImage("C:/Users/USUARIO/Desktop/ITBA/Algoritmos y estructuras de datos/Ant Simulation/resources/map1.png");

    Texture2D mapTexture = LoadTextureFromImage(map);    

    bool textureFlop = false;
    RenderTexture2D mapRenderTexture[2];
    for (int i = 0; i < 2; i++)
    {
        mapRenderTexture[i] = LoadRenderTexture(screenWidth, screenHeight);
        BeginTextureMode(mapRenderTexture[i]);
        ClearBackground(BLACK);
        EndTextureMode();
    }

    std::vector<pheromones> pheromoneMap;
    pheromoneMap.resize(screenWidth * screenHeight+10000,  { 0, 0 });



    /*for (int i = 0; i < 3000; i++)
        ant[i].hasFood = 1;*/

    int checkIlegalAnt = 0;
    raylib::Color blackColor = BLACK;

    int pheromoneTimer = 0;

    while (!WindowShouldClose())
    {

        /*Color colorMouse = GetImageColor(map,GetMousePosition().x, GetMousePosition().y);        
        cout << (int)colorMouse.r << " " << (int)colorMouse.g << " " << (int)colorMouse.b << endl;*/

        cout << "Home: " << pheromoneMap[GetMousePosition().y * screenWidth + GetMousePosition().x].home;
        cout << " Food: " << pheromoneMap[GetMousePosition().y * screenWidth + GetMousePosition().x].food << endl;

        for (auto& i : ant)
        {
            i.update(TIME_STEP, map, pheromoneMap);
        }

        /*checkIlegalAnt++;
        if (checkIlegalAnt > 20)
        {
            for (auto& i : ant)
            {
                if ((raylib::Color)GetImageColor(map, (int)i.position.x, (int)i.position.y) == blackColor)
                {
                    i.position = { screenWidth / 2,screenHeight / 2 };
                    cout << "HORMIGA ILEGAL!! " << endl;
                    
                }
            }
        }*/

        pheromoneTimer++;
        if (pheromoneTimer > DIFUSION_TIMER)
        {
            for (auto& i : pheromoneMap)
            {
                if (i.food > 0)
                    i.food -= 1;
                if (i.home > 0)
                    i.home -= 1;
            }
            pheromoneTimer = 0;
        }
            
         
        
        
        textureFlop = !textureFlop;

        int lastFrame = textureFlop ? 1 : 0;
        int thisFrame = textureFlop ? 0 : 1;

        BeginTextureMode(mapRenderTexture[thisFrame]);
        DrawTextureRec(mapTexture, Rectangle{ 0,0, screenWidth, screenHeight }, Vector2Zero(), ColorAlpha(WHITE, 1));
        DrawTextureRec(mapRenderTexture[lastFrame].texture, Rectangle{ 0,0, screenWidth, -screenHeight }, Vector2Zero(), ColorAlpha(WHITE, 0.999f));

        /*for (auto& i : ant)               //ant trail in texture
        {
            if(i.foodPheromonePower>0)
                DrawPixelV(i.position, GREEN);
            else if(i.homePheromonePower>0)
                DrawPixelV(i.position, RED);
        }*/
       
        for(int x=0;x<1000;x++)             //esto anda bien
            for (int y = 0; y < 600; y++)
            {
                if (pheromoneMap[y * 1000 + x].home >0 )
                    DrawPixel(x, y, ColorAlpha(PURPLE,float(pheromoneMap[y * 1000 + x].home)));
            }
        for (int x = 0; x < 1000; x++)
            for (int y = 0; y < 600; y++)
            {
                if (pheromoneMap[y * 1000 + x].food > 0)
                    DrawPixel(x, y, ColorAlpha(BLUE, pheromoneMap[y * 1000 + x].food));
            }


        EndTextureMode();

        BeginDrawing();
        ClearBackground(WHITE);

        DrawTextureRec(mapRenderTexture[thisFrame].texture, Rectangle{ 0,0, screenWidth, -screenHeight }, Vector2Zero(), WHITE);

        for (auto &i : ant)
        {
            DrawPixelV(i.position, BLACK);
        }

        EndDrawing();

    }

    UnloadImage(map);
    UnloadTexture(mapTexture);
    UnloadRenderTexture(mapRenderTexture[0]);
    UnloadRenderTexture(mapRenderTexture[1]);


    CloseWindow();


    return 0;
}


