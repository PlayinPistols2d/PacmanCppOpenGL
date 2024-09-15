#include "glew.h" 
#include "glut.h"
#include "pill.h"
#include "texture.h"

Pill::Pill() 
{
     pillX=0;
     pillY=0;
}

void Pill::drawPill()
{
    drawTex(bigPill_tex, 2, 2, 0);
}