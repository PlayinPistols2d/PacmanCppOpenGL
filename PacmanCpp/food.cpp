#include "glew.h" 
#include "glut.h"
#include "food.h"
#include "texture.h"

Food::Food()
{
    foodX=0;
    foodY=0;
}

void Food::drawfood()
{
    drawTex(pill_tex, 2, 2, 0);
}