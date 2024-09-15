#pragma once
#ifndef pacman_h
class Pacman
{
public:
    float pacmanX;
    float pacmanY;
    float speed;
    int pDirection;
    int buffDirection;
    float angle;

    Pacman();
    int getPacmanX();
    int getPacmanY();
    int getTile(float, float);
    int CheckAhead(int);
    bool isWall(int);
    bool isAtCenter();
    void setBufferDirection(int);
    void move();
    void detectGhost();
    void death();
    void reset();
    void draw();
};
#endif