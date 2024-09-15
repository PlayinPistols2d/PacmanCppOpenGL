#include "glew.h" 
#include "glut.h"
#include "cmath"
#include <math.h>
#include <time.h>
#include <iostream>
using namespace std;
#include "ghost.h"
#include "map.h"
#include "pacman.h"
#include "texture.h"
#include "interface.h"

bool gameOver = false;
extern int Score;
extern int timestamp;
extern int gameTick;
extern int eatenCount;
extern int eatStamp;
extern int MAINSCORE;
extern bool gameBegin;
extern int deathTick;
typedef enum { BEGIN, PLAY, DIE, OVER ,WIN} gameState;
extern gameState stateGame;

#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2
#define NONE 0
int pacTick = 0;
float eatenX;
float eatenY;
extern bool dead;
extern int lives;
extern Ghost ghosts[4];
extern Map map;

Pacman::Pacman()
{
    pacmanX = 13.0f;
    pacmanY = 7.0f;
    speed = 1.0f;
    pDirection = NONE;
    buffDirection = NONE;
}

int Pacman::getPacmanX()
{
    return (int)floor(pacmanX);
}
int Pacman::getPacmanY()
{
    return (int)floor(pacmanY);
}

int  Pacman::getTile(float x, float y)
{
    return map.mapMatrix[(int)floor(x)][(int)floor(y)];
}


int Pacman::CheckAhead(int pDirection)
{
    switch (pDirection)
    {
    case -2: return map.mapMatrix[getPacmanX() - 1][getPacmanY()];
    case 1: return map.mapMatrix[getPacmanX()][getPacmanY() + 1];
    case 2: return map.mapMatrix[getPacmanX() + 1][getPacmanY()];
    case -1: return map.mapMatrix[getPacmanX()][getPacmanY() - 1];
    default: return map.mapMatrix[getPacmanX()][getPacmanY()];
    }
}

bool Pacman::isWall(int pDirection)
{
    if (CheckAhead(pDirection) == 1 || CheckAhead(pDirection) == 5) {
        return true;
    }
    return false;
}

void Pacman::detectGhost()
{
    for (int i = 0; i < 4; i++) { // Check pacmans position against all of the ghosts
        if (getPacmanX() == ghosts[i].getGhostX() && getPacmanY() == ghosts[i].getGhostY()) { // If he is on the same tile as any ghost
            if (ghosts[i].ghostState != FRIGHTEN && ghosts[i].ghostState != DEATH) { // Check if they are in a mode which is damaging to pacman
                stateGame = DIE; // Execute the DIE gameType as the ghost has caught pacman
                timestamp = gameTick;
            }
            else {
                if (!ghosts[i].eaten) { // Check the ghost hasnt already been eaten
                    eatenCount++;
                    eatStamp = gameTick;
                    switch (eatenCount) { // Check what eaten number the ghost is in order to increment the score correctly
                    case 1: Score += 200;
                        break;
                    case 2: Score += 200;
                        break;
                    case 3: Score += 400;
                        break;
                    case 4: Score += 800;
                        break;
                    }
                    eatenX = ghosts[i].ghostX; // Store the position of the eating in order to draw the eating score tex in the correct place
                    eatenY = ghosts[i].ghostY;
                    ghosts[i].eaten = true; // Set the flag for that specifc ghost to have been eaten so it cannot be eaten again in this frighten cycle
                }
                ghosts[i].ghostState = DEATH; // Set the ghosts mode to DEATh as it has been eaten
            }
        }
    }
    if (stateGame == DIE && !dead) { // Check to see if pacman was indeed caught by the ghost and if so take away a life
        lives--;
        dead = true;
        if (lives == 1)
        {

        }
       // cout << "dead--";
    }
}

bool Pacman::isAtCenter()
{
    return (int)(pacmanY * 10.0f) % 10 == 0 && (int)(pacmanX * 10.0f) % 10 == 0;
}

void  Pacman::setBufferDirection(int pDirection)
{
    buffDirection = pDirection;
}

void Pacman::reset()
{
    pacmanX = 13.5f;
    pacmanY = 7.0f;
    pDirection = NONE;
    buffDirection= NONE;
    gameBegin = true;
}

void Pacman::draw()
{
    glPushMatrix();

    unsigned int pac_tex;
    int tickDiv;

    if (!dead) {
        tickDiv = pacTick % 30;
    }
    if (pDirection == NONE) {
        tickDiv = 0;
    }
    if (gameTick < 250) { // If the game is not in BEGIN mode
        pac_tex = pac_2_tex;
    }
    else {
        if (tickDiv % 30 < 10) {
            pac_tex = pac_0_tex;
        }
        else if (tickDiv % 30 < 20) {
            pac_tex = pac_1_tex;
        }
        else {
            pac_tex = pac_2_tex;
        }
    }

    switch (pDirection) { // Rotates pacman to face in his current travel direction
    case LEFT: angle = 0.0f;
        break;
    case UP: angle = 270.0f;
        break;
    case RIGHT: angle = 180.0f;
        break;
    case DOWN: angle = 90.0f;
        break;
    default: pDirection = NONE;
    }
    if (stateGame == WIN)
    {
        pac_tex = pac_happy;
        angle = 0;
    }
    glTranslatef(pacmanX, pacmanY, 1.0f);
    drawTex(pac_tex, 2, 2, angle);

    glPopMatrix();
    pacTick++;
}

void Pacman::death()
{
    glPushMatrix();

    unsigned int pac_tex;

    if (deathTick < 10) { // Depending on the deathTick, assign the corresponding death sprite
        pac_tex = dead_0_tex;
    }
    else if (deathTick % 110 < 20) {
        pac_tex = dead_1_tex;
    }
    else if (deathTick % 110 < 30) {
        pac_tex = dead_2_tex;
    }
    else if (deathTick % 110 < 40) {
        pac_tex = dead_3_tex;
    }
    else if (deathTick % 110 < 50) {
        pac_tex = dead_4_tex;
    }
    else if (deathTick % 110 < 60) {
        pac_tex = dead_5_tex;
    }
    else if (deathTick % 110 < 70) {
        pac_tex = dead_6_tex;
    }
    else if (deathTick % 110 < 80) {
        pac_tex = dead_7_tex;
    }
    else if (deathTick % 110 < 90) {
        pac_tex = dead_8_tex;
    }
    else if (deathTick % 110 < 100) {
        pac_tex = dead_9_tex;
    }
    else {
        pac_tex = dead_10_tex;
    }
    glTranslatef(pacmanX, pacmanY,1.0f);
    drawTex(pac_tex, 2, 2, 0);
    glPopMatrix();


}

void Pacman::move()
{
    map.fruitCheck(pacmanX, pacmanY);
    map.pillCheck(pacmanX, pacmanY);
    if (isAtCenter()) { // Only move if pacman is at the centre of the tile
        if (!isWall(buffDirection)) { // If the direction pacman wants to go in is not a wall, set that direction
            pDirection = buffDirection;
        }
        else if (isWall(pDirection)) {
            pDirection = NONE; // If the direction pacman wants to travel in is a wall and the direction he is currently travelling is a wall, stop his movement
        }
    }
    if (!isAtCenter() && buffDirection != NONE && gameBegin && !isWall(buffDirection)) { // Check to not cause pacman to jump at the very beginning of the game
        pDirection = buffDirection;
        gameBegin = false;
    }

    if (getTile(pacmanX, pacmanY) == 3)
    {
        if (getPacmanX() >=
            26.5) { // If pacman is on a portal tile, translate his position to the other side of the map
            pacmanX = 1.0f;
        }
        else {
            pacmanX = 26.5f;
        }
    }

    switch (pDirection) {
    case LEFT:
        pacmanX -= 0.1f;
        pacmanY = round(pacmanY);
        break;
    case UP:
        pacmanY += 0.1f;
        pacmanX = round(pacmanX);
        break;
    case RIGHT:
        pacmanX += 0.1f;
        pacmanY = round(pacmanY);
        break;
    case DOWN:
        pacmanY -= 0.1f;
        pacmanX = round(pacmanX);
        break;
    default:
        if (!gameBegin) { // Do not round pacman if it is the very start as he begin between two tiles
            pacmanX = round(pacmanX);
            pacmanY = round(pacmanY);
        }
    }
}

