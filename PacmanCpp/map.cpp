#include "glew.h" 
#include "glut.h"
#include <math.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include <png.h>
#include <stdlib.h>
#include <vector>
#include <fstream>

#include "map.h"
#include "ghost.h"
#include "food.h"
#include "pill.h"
#include "pacman.h"
#include <iostream>
#include "texture.h"
using namespace std;
extern Ghost ghosts[4];
extern Pacman pacman;
extern Food mfood;
extern Pill mpill;
extern int Score;
extern bool frighten;
extern int frightenTick;
extern int ghostScore;
extern int fruitScore;
typedef enum { BEGIN, PLAY, DIE, OVER } gameState;
extern gameState stateGame;

#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4 //1-up,2-down,3-right,4-left
#define NONE 0
int mapbuff[28][31];
//1-wall,0-food,9-pillState,8-wallback,5-ghostblock,3-teleport
Map::Map() : mapMatrix{ {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
                        {1,0,0,0,0,1,1,9,0,0,0,1,8,8,8,1,8,1,8,8,8,1,0,0,0,0,9,0,0,0,1},
                        {1,0,1,1,0,1,1,0,1,1,0,1,8,8,8,1,8,1,8,8,8,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,0,0,0,1,1,0,1,8,8,8,1,8,1,8,8,8,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,1,1,1,1,1,0,1,8,8,8,1,8,1,8,8,8,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,8,1,1,1,1,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,8,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
                        {1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,8,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,0,0,0,1,1,0,8,8,8,8,8,8,8,8,8,1,1,0,0,0,0,1,1,1,0,1},
                        {1,0,1,1,0,1,1,0,1,1,0,1,1,8,1,1,1,1,1,8,1,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,1,1,0,1,1,0,1,1,8,1,8,8,8,1,8,1,1,0,1,1,0,1,1,1,0,1},
                        {1,0,0,0,0,1,1,0,0,0,0,1,1,8,1,8,8,8,1,8,8,8,0,1,1,0,0,0,0,0,1},
                        {1,0,1,1,1,1,1,8,1,1,1,1,1,8,1,8,8,8,5,8,1,1,1,1,1,0,1,1,1,1,1},
                        {1,0,1,1,1,1,1,8,1,1,1,1,1,8,1,8,8,8,5,8,1,1,1,1,1,0,1,1,1,1,1},
                        {1,0,0,0,0,1,1,0,0,0,0,1,1,8,1,8,8,8,1,8,8,8,0,1,1,0,0,0,0,0,1},
                        {1,0,1,1,0,1,1,0,1,1,0,1,1,8,1,8,8,8,1,8,1,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,1,1,0,1,1,0,1,1,8,1,1,1,1,1,8,1,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,0,0,0,1,1,0,8,8,8,8,8,8,8,8,8,1,1,0,0,0,0,1,1,1,0,1},
                        {1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,8,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
                        {1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,8,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
                        {1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,8,1,1,1,1,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,1,1,1,1,1,0,1,8,8,8,1,8,1,8,8,8,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,0,0,0,1,1,0,1,8,8,8,1,8,1,8,8,8,1,0,1,1,0,1,1,1,0,1},
                        {1,0,1,1,0,1,1,0,1,1,0,1,8,8,8,1,8,1,8,8,8,1,0,1,1,0,1,1,1,0,1},
                        {1,0,0,0,0,1,1,9,0,0,0,1,8,8,8,1,8,1,8,8,8,1,0,0,0,0,9,0,0,0,1},
                        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}
{}

int Map::locationCheck(int x, int y)
{
    return mapMatrix[x][y];
}

void Map::fruitReset()
{
    for (int x = 0; x < 28; x++) {            // Iterate through the maze
        for (int y = 0; y < 31; y++) {
            switch (mapMatrix[x][y]) {
            case 4:
                mapMatrix[x][y] = 6; // If a fruit tile is found reset it
                break;
            }
        }
    }
}

void Map::fruitNewGame()
{
    for(int x=0;x<28;x++) {            // Iterate through the maze
        for(int y=0;y<31;y++) {
            switch(mapMatrix[x][y]) {
                case 0:
                    mapMatrix[x][y] = 0; // If a fruit tile is found reset it
                    break;
                case 6:
                    mapMatrix[x][y] = 0; // If a fruit tile is found reset it
                    break;
            }
        }
    }
}

void Map::fruitCheck(int x, int y)
{
    if (locationCheck((int)floor(x), (int)floor(y)) == 0)
    {
        mapMatrix[(int)floor(x)][(int)floor(y)] = 4; //buf
        fruitReset();
        Score+=10;
        fruitScore++;
        ghostScore++;
    }
}

void Map::pillReset()
{
    for (int x = 0; x < 28; x++) {            // Iterate through the maze
        for (int y = 0; y < 31; y++) {
            switch (mapMatrix[x][y]) {
            case 7:
                mapMatrix[x][y] = 2; // If a fruit tile is found reset it
                break;
            }
        }
    }
}

void Map::pillNewGame()
{
    for (int x = 0; x < 28; x++) {            // Iterate through the maze
        for (int y = 0; y < 31; y++) {
            switch (mapMatrix[x][y]) {
            case 2:
                mapMatrix[x][y] = 9; // If a fruit tile is found reset it
                break;
            }
        }
    }
}

void Map::pillCheck(int x, int y)
{
    if (locationCheck((int)floor(x), (int)floor(y)) == 9)
    {
        mapMatrix[(int)floor(x)][(int)floor(y)] = 7;
        pillReset();
        frighten = true; // Set the flag to know that frigthen mode is on
        frightenTick = 0; // Set the counter to 0
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState == CHASE || ghosts[i].ghostState == SCATTER) { // Only change the ghosts mode if they are in CHASE or SCATTER mode
                ghosts[i].eaten = false;
                ghosts[i].ghostState = FRIGHTEN;
                if (ghosts[i].gDirection == LEFT) { // Reverse their direction
                    ghosts[i].gDirection = RIGHT;
                }
                else if (ghosts[i].gDirection == UP) {
                    ghosts[i].gDirection = DOWN;
                }
                else if (ghosts[i].gDirection == RIGHT) {
                    ghosts[i].gDirection = LEFT;
                }
                else {
                    ghosts[i].gDirection = UP;
                }
            }
        }
    }
}

void Map::drawMap()
{
    glPushMatrix();
    drawTex(maze_tex, 29, 32, 0);
    for (int x = 0; x < 28; x++) {
        glPushMatrix();
        for (int y = 0; y < 31; y++) {
            switch (mapMatrix[x][y]) {
            case 0:
                mfood.drawfood();
                break;
            case 9:
                mpill.drawPill();
                break;
            case 1:
                break;

            case 3:
                glColor3f(0.0, 0.0, 0.0);
                glRectd(x, y, x + 1, y + 1);
                break;
            }
            glTranslatef(0.0f, 1.0f, 0.0f); // Increment Y position
        }
        glPopMatrix();
        glTranslatef(1.0f, 0.0f, 0.0f);     // Increment X position
    }
    glPopMatrix();
}