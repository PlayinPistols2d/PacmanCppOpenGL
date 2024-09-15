#pragma once
typedef enum { NORMAL, ATTACK } state;

class Pill
{
public:
    int pillX;
    int pillY;

    Pill();
    void drawPill();
};