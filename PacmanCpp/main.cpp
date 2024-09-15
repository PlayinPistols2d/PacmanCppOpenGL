#include "glew.h" 
#include "glut.h"
#include "windows.h"
#include "cmath"
#include <math.h>
#include <time.h>
#include <iostream>
using namespace std;

#include "pacman.h"
#include "map.h"
#include "pill.h"
#include "food.h"
#include "ghost.h"
#include "texture.h"
#include "interface.h"
#define COLUMNS 28
#define ROWS 32
#define FPS 1
#define UP 1
#define DOWN -1
#define RIGHT 2
#define LEFT -2

typedef enum { BEGIN, PLAY, DIE, OVER, WIN } gameState;
gameState stateGame = BEGIN;
Pacman pacman;
Ghost ghosts[4] = {
        Ghost(13.5f, 19.0f, RED),
        Ghost(11.5f, 16.0f, BLUE),
        Ghost(13.5f, 16.0f, PINK),
        Ghost(15.5f, 16.0f, YELLOW),
};
Map map;
Food mfood;
Pill mpill;
bool frighten=false;
bool gameBegin = true;
bool firstTry=false;
int frightenTick = 0;
int timestamp;
int gameTick = 0;
int eatenCount = 0;
int eatStamp;
int MAINSCORE = 0;
int deathTick = 0;
bool paused = false;
extern bool gameOver;
int Score = 0;
int lives = 3;
int eatFruitStamp = 0;
bool dead = false;
int ghostScore = 0;
int fruitScore = 0;
bool stop = false;
bool deathProc = false;
int pacmanDeathCount = 0;
int previousScore=0;

void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 300, 0, 300);
    loadAndBindTextures();
}

void setMode()
{
    if (gameTick >= 4450) {
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState != PEN && ghosts[i].ghostState != LEAVE && !frighten && ghosts[i].ghostState != DEATH) {
                ghosts[i].ghostState = CHASE;
            }
        }
    }
    else if (gameTick >= 4200) {
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState != PEN && ghosts[i].ghostState != LEAVE && !frighten && ghosts[i].ghostState != DEATH) {
                ghosts[i].ghostState = SCATTER;
            }
        }
    }
    else if (gameTick >= 3200) {
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState != PEN && ghosts[i].ghostState != LEAVE && !frighten && ghosts[i].ghostState != DEATH) {
                ghosts[i].ghostState = CHASE;
            }
        }
    }
    else if (gameTick >= 2950) {
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState != PEN && ghosts[i].ghostState != LEAVE && !frighten && ghosts[i].ghostState != DEATH) {
                ghosts[i].ghostState = SCATTER;
            }
        }
    }
    else if (gameTick >= 1950) {
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState != PEN && ghosts[i].ghostState != LEAVE && !frighten && ghosts[i].ghostState != DEATH) {
                ghosts[i].ghostState = CHASE;
            }
        }
    }
    else if (gameTick >= 1600) {
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState != PEN && ghosts[i].ghostState != LEAVE && !frighten && ghosts[i].ghostState != DEATH) {
                ghosts[i].ghostState = SCATTER;
            }
        }
    }
    else if (gameTick >= 600) {
        for (int i = 0; i < 4; i++) {
            if (ghosts[i].ghostState != PEN && ghosts[i].ghostState != LEAVE && !frighten && ghosts[i].ghostState != DEATH) {
                ghosts[i].ghostState = CHASE;
            }
        }
    }
}

void EventOnDeath()
{
    glClear(GL_COLOR_BUFFER_BIT);
    switch (stateGame) { // Check what state the game is in
    case DIE:
        map.drawMap();
        drawInterface();
        if (paused)
        {
            drawPause();
            drawUnpauseHint();
        }
        else
        {
            drawPauseHint();
        }

        if (pacmanDeathCount >= 50 && !stop) { // Freeze pacman momentarily after he is hit by a ghost               //150
            stop = true;
            pacmanDeathCount = 0;
        }
        if (gameTick - timestamp >= 150 && deathTick < 110) { // After that time has passed play pacmans death animation
            deathProc = true;
            pacman.death();
        }
        if (gameTick - timestamp >= 310) { // After the death animation, reset pacman                           //310
            deathProc = false;
            pacman.reset();
            for (int i = 0; i < 4; i++) {
                ghosts[i].reset(); // Reset the ghosts also
            }
            gameTick = 0;
            eatFruitStamp = -999999999;
            deathTick = 0;
            if (lives > 0) { // If pacman has lives left begin the game again
                stateGame = BEGIN;
            }
            else { // Otherwise show the gameover screen
                stateGame = OVER;
            }
        }
        break;
    case OVER:
        map.drawMap();
       // previousScore = Score;
        break;
    case WIN:
        map.drawMap();
        //previousScore = Score;
        pacman.pDirection = 0;
        for (int i = 0; i < 4; i++) {
            ghosts[i].gDirection=0;
        }
        break;
    }
    stop = false;
    pacmanDeathCount = 0;
    glutSwapBuffers(); // Force a redraw at the end
}

void idle()
{
    if (!paused) {
        switch (stateGame) { 
        case BEGIN:
            if (gameTick >= 250) 
            { // timer
                dead = false;
                stateGame = PLAY;
            }
            break;
        case PLAY:
            setMode();
            pacman.move();
            pacman.detectGhost();
            for (int i = 0; i < 4; i++) { // ghosts movement
                ghosts[i].move(ghosts[0]);
            }
            pacman.detectGhost();
            if (frightenTick <= 450 && frighten) { //If the fear mode is on and has not expired, the counter is incremented
                frightenTick++;
            }
            else if (frighten) { // If fear mode is enabled and its duration expires, the counter is reset.
                frighten = false;
                eatenCount = 0;
                frightenTick = 0;
                for (int i = 0; i < 4; i++) {
                    if (ghosts[i].ghostState == FRIGHTEN) {
                        ghosts[i].eaten = false;
                    }
                }
                setMode(); // Returns ghosts to their correct mode after finishing their scare mode
            }
            if (fruitScore >= 240) { // all fruits done - restart
                stateGame = WIN;
            }
            break;
        case DIE:
            if (gameTick - timestamp >= 150) { // pacman death
                deathTick++;
                EventOnDeath();
                ghostScore = 0;;
                firstTry = true;
            }
            break;
        case WIN:
            previousScore = Score;
            break;
        case OVER:
            previousScore = Score;
            break;
        }
        gameTick++;

        glutPostRedisplay();
    }
}


void display_callback()
{
    glClear(GL_COLOR_BUFFER_BIT);
    map.drawMap();
    if (paused)
    {
        drawPause();
        drawUnpauseHint();
    }
    else
    {
        drawPauseHint();
    }
    drawInterface();
        if (!dead)
        {     
            pacman.draw();
            for (int i = 0; i < 4; i++) {
                ghosts[i].draw();
            }
        }
        switch (stateGame) { // Check what state the game is in
        case BEGIN:
            if (!paused) {
                drawReady();
            }
            pacman.draw();
            for (int i = 0; i < 4; i++) {
                ghosts[i].draw();
            }
            break;
        case OVER:
            setHighScore(Score);
            if (!paused) {
                drawGameOver();
                drawRestartHint();
            }
            break;
        case WIN:
            setHighScore(Score);
            if (!paused){
            drawYouWon();
            drawRestartHint();
            }
        case DIE:
            if (stop==false)
            {
                pacmanDeathCount++;
            }
        }
        idle();
    glutSwapBuffers();
}

void resetGame()
{
    gameTick = 0;
    deathTick = 0;
    Score = 0;
    lives = 3;
    fruitScore = 0;
    stateGame = BEGIN;
    ghostScore = 0;
}

void reshape_callback(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double ww = width, hh = height;
    if (width > height)
        glOrtho(0.0 * (ww / hh), COLUMNS * (ww / hh), 0.0, ROWS, -1.0, 1.0);
    else
        glOrtho(0.0, COLUMNS, 0.0 * (hh / ww), ROWS * (hh / ww), -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard_callback(int key, int, int)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        pacman.setBufferDirection(UP);
        break;
    case GLUT_KEY_DOWN:
        pacman.setBufferDirection(DOWN);
        break;
    case GLUT_KEY_RIGHT:
        pacman.setBufferDirection(RIGHT);
        break;
    case GLUT_KEY_LEFT:
        pacman.setBufferDirection(LEFT);
        break;
    }
    glutPostRedisplay();
}

void keyboard(unsigned char key, int, int)
{
    switch (key)
    {
    case 'q':
        exit(1);
        break;
    case 'r':
        if (stateGame == OVER || stateGame ==WIN) {
            previousScore = Score;
            setPreviousScore(previousScore);
            resetGame();
            pacman.reset();
            map.fruitNewGame();
            map.pillNewGame();
            for (int i = 0; i < 4; i++) {
                ghosts[i].reset();
            }
        }
        break;
    case 'p':
        if (paused) {
            paused = false;
        }
        else {
            if (!deathProc)
            {
                paused = true;
            }
        }
        break;
    }

    glutPostRedisplay(); // force a redraw
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1200, 640);
    glutCreateWindow("Pacman");
    glutDisplayFunc(display_callback);
    glutReshapeFunc(reshape_callback);
    glutSpecialFunc(keyboard_callback);
    glutKeyboardFunc(keyboard);
    init();
    glutMainLoop();
    return 0;
}