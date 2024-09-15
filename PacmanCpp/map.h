#pragma once
#ifndef map_h
class Map
{
public:
    static const int SizeX = 28;
    static const int SizeY = 31;

    Map();
    void drawMap();
    int locationCheck(int, int);
    void fruitCheck(int, int);
    void pillCheck(int, int);
    void fruitReset();
    void pillReset();
    void fruitNewGame();
    void pillNewGame();

public:
    int mapMatrix[SizeX][SizeY];
};
#endif