#pragma once
#include <vector>
using namespace std;
#ifndef ghost_h

typedef enum { SCATTER, CHASE, FRIGHTEN, LEAVE, DEATH, DEFAULT,PEN } movement;
typedef enum { RED, BLUE, PINK, YELLOW } colour;
class Ghost
{
public:
    float ghostX;
    float ghostY;
    int gDirection;
    movement ghostState;
    bool eaten;
    colour ghostColour;
    float speed;

public:
    Ghost(float, float, colour);
    int getGhostX();
    int getGhostY();
    int getTile(float, float);
    int CheckAhead(int);
    bool isWall(int);
    bool isOnlyWall(int);
    bool isAtCenter();
    int checkAllDirections(float, float);
    void cornerHandler();
    float getDistance(int, int, colour, Ghost);
    void setSpeed();
    vector<int> getTargetTile(colour);
    void moveTargetTile(colour, Ghost);
    void portalCheck();
    void redGhostMove(Ghost);
    void blueGhostMove(Ghost);
    void yellowGhostMove(Ghost);
    void pinkGhostMove(Ghost);
    void penMove();
    void leaveMove();
    void setMove(colour, Ghost);
    void move(Ghost);
    void setFrighten();
    void moveToPen();
    void reset();
    void setDeath(colour, Ghost);
    void draw();
};
#endif