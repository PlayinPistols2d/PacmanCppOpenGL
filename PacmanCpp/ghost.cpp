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
#include <vector>
#include "texture.h"
short gDirectionValue = 0;
extern Map map;
extern Pacman pacman;
extern int Score;
extern int ghostScore;
extern int gameTick;
#define UP 1
#define DOWN 2
#define RIGHT 3
#define LEFT 4 //1-up,2-down,3-right,4-left
#define NONE 0
int tempDir = NONE;

Ghost::Ghost(float x, float y, colour ghost) {
    ghostX = x;
    ghostY = y;
    switch (ghost) {
    case RED:
        gDirection = LEFT;
        ghostState = SCATTER;
        break;
    case PINK:
        gDirection = DOWN;
        ghostState = PEN;
        break;
    case BLUE:
        gDirection = UP;
        ghostState = PEN;
        break;
    case YELLOW:
        gDirection = UP;
        ghostState = PEN;
        break;
    }
    eaten = false;
    ghostColour = ghost;
    setSpeed();
}

void Ghost::setSpeed() {
    if (ghostState == FRIGHTEN) { // move slower if in frighten mode
        speed = 0.05f;
    }
    else {
        speed = 0.1f;
    }
}

void Ghost::move(Ghost ghost) {
    setSpeed();
    switch (ghostColour) {
    case RED:
        redGhostMove(ghost);
        break;
    case BLUE:
        blueGhostMove(ghost);
        break;
    case PINK:
        pinkGhostMove(ghost);
        break;
    case YELLOW:
        yellowGhostMove(ghost);
        break;
    }
}

void Ghost::setMove(colour col, Ghost ghost)
{
    float distance = getDistance(ghostX, ghostY, RED, ghost); // gets the distance between ghosts and pacman
    switch (col) {
    case RED:
        moveTargetTile(RED, ghost);
        break;
    case PINK:
        moveTargetTile(RED, ghost);
        break;
    case YELLOW:
        if (distance >= 8.0f) {
            moveTargetTile(RED, ghost);
        }
        else {
            moveTargetTile(YELLOW, ghost);
        }
        break;
    case BLUE:
        moveTargetTile(BLUE, ghost);
        break;
    }
}

void Ghost::moveToPen()
{
    if (ghostX <= 13.55 && ghostX >= 13.45) { // from the start of the game checks the correctness of the X coordinates
        if (ghostY >= 18.95f) { //  moves them up out of the gate
            gDirection = DOWN;
        }
        else if (ghostY <= 17.0f) {
            ghostState = LEAVE;
        }
    }
    else { //If they don't have the correct x coordinates
        if (ghostX < 13.5f) { 
            gDirection = RIGHT;
        }
        else if (ghostX > 13.5f) {
            gDirection = LEFT;
        }
    }
  // Since gDirection is changed on the last call to this method, they can still move 0.1f in a direction when they shouldn't, to avoid this you need to check gDirection
    switch (gDirection) {
    case LEFT:
        ghostX -= speed;
        break;
    case UP:
        ghostY += speed;
        break;
    case RIGHT:
        ghostX += speed;
        break;
    case DOWN:
        ghostY -= speed;
        break;
    }
}

void Ghost ::setDeath(colour colGhost, Ghost ghost)
{
    float distance = 999.0f;
    bool move = true;
    if (ghostX <= 13.55 && ghostX >= 13.45 && ghostY >= 15.5f && ghostY <= 20.0f) {
        moveToPen();
    }
    else {
        if (checkAllDirections(ghostX, ghostY) >= 3 && isAtCenter()) {
            if (gDirection != DOWN && !isOnlyWall(UP)) {
                float d = getDistance(getGhostX(), getGhostY() + 1, colGhost, ghost);
                if (d < distance) {
                    tempDir = UP;
                    distance = d;
                }
            }
            if (gDirection != LEFT && !isOnlyWall(RIGHT)) {
                float d = getDistance(getGhostX() + 1, getGhostY(), colGhost, ghost);
                if (d < distance) {
                    tempDir = RIGHT;
                    distance = d;
                }
            }
            if (gDirection != RIGHT && !isOnlyWall(LEFT)) {
                float d = getDistance(getGhostX() - 1, getGhostY(), colGhost, ghost);
                if (d < distance) {
                    tempDir = LEFT;
                    distance = d;
                }
            }
            if (gDirection != UP && !isOnlyWall(DOWN)) {
                float d = getDistance(getGhostX(), getGhostY() - 1, colGhost, ghost);
                if (d < distance) {
                    tempDir = DOWN;
                    distance = d;
                }
            }
            gDirection = tempDir;
        }
        else {
            cornerHandler();
        }

        if (move) {
            switch (gDirection) {
            case LEFT:
                ghostX -= speed;
                ghostY = round(ghostY);
                break;
            case UP:
                ghostY += speed;
                ghostX = round(ghostX);
                break;
            case RIGHT:
                ghostX += speed;
                ghostY = round(ghostY);
                break;
            case DOWN:
                ghostY -= speed;
                ghostX = round(ghostX);
                break;
            }
        }
    }
};

void Ghost::setFrighten()
{
    int chosenDir = NONE;
    if (checkAllDirections(ghostX, ghostY) >= 3) { // If you are at a dead end
        do {
            int randNum = rand() % checkAllDirections(ghostX, ghostY) - 1; // Pick a random number between 0 and 3
            if (randNum == 0 && gDirection != RIGHT) {
                chosenDir = LEFT;
            }
            else if (randNum == 1 && gDirection != DOWN) {
                chosenDir = UP;
            }
            else if (randNum == 2 && gDirection != LEFT) {
                chosenDir = RIGHT;
            }
            else if (gDirection != UP) {
                chosenDir = DOWN;
            }
        } while (isWall(chosenDir) && gDirection != NONE); // Selects a direction only if it is not facing a wall and the ghost is currently moving
    }
    else {
        cornerHandler();
    }

    if (isAtCenter() && chosenDir != NONE) {
        if (!isWall(chosenDir)) {
            gDirection = chosenDir; // Assigns the chosen direction only if the ghost is in the center of the cell, the chosen direction becomes NONE and does not lead him into the wall.
        }
    }


    switch (gDirection) { // Gradually moves the ghost in the selected direction according to its speed.
    case LEFT:
        ghostX -= speed;
        ghostY = round(ghostY);
        break;
    case UP:
        ghostY += speed;
        ghostX = round(ghostX);
        break;
    case RIGHT:
        ghostX += speed;
        ghostY = round(ghostY);
        break;
    case DOWN:
        ghostY -= speed;
        ghostX = round(ghostX);
        break;
    }
}

void Ghost::redGhostMove(Ghost ghost)
{
    switch (ghostState) {
    case SCATTER:
        portalCheck();
        setMove(RED, ghost);
        break;
    case CHASE:
        portalCheck();
        setMove(RED, ghost);
        break;
    case FRIGHTEN:
        portalCheck();
        setFrighten();
        break;
    case DEATH:
        setDeath(RED, ghost);
        break;
    case LEAVE:
        leaveMove();
        break;
    }
}

/**
* Calls the correct movement method based on the ghost's current gDirection.
* the ghost parameter is used for specific movement patterns for specific ghosts
*/
void Ghost::blueGhostMove(Ghost ghost)
{
    if (ghostScore == 30) { //163
        ghostState = LEAVE;
    }
    switch (ghostState) {
    case SCATTER:
        portalCheck();
        setMove(BLUE, ghost);
        break;
    case CHASE:
        portalCheck();
        setMove(BLUE, ghost);
        break;
    case FRIGHTEN:
        portalCheck();
        setFrighten();
        break;
    case DEATH:
        setDeath(BLUE, ghost);
        break;
    case LEAVE:
        leaveMove();
        break;
    case PEN: penMove();
        break;
    }
}

/**
* Calls the correct movement method based on the ghost's current gDirection.
* the ghost parameter is used for specific movement patterns for specific ghosts
*/
void Ghost::yellowGhostMove(Ghost ghost)
{
    if (ghostScore == 80) { //214
        ghostState = LEAVE;
    }
    switch (ghostState) {
    case SCATTER:
        portalCheck();
        setMove(YELLOW, ghost);
        break;
    case CHASE:
        portalCheck();
        setMove(YELLOW, ghost);
        break;
    case FRIGHTEN:
        portalCheck();
        setFrighten();
        break;
    case DEATH: setDeath(YELLOW, ghost);
        break;
    case LEAVE: leaveMove();
        break;
    case PEN: penMove();
        break;
    }
}

/**
* Calls the correct movement method based on the ghost's current gDirection.
* the ghost parameter is used for specific movement patterns for specific ghosts
*/
void Ghost::pinkGhostMove(Ghost ghost)
{
    switch (ghostState) {
    case SCATTER:
        portalCheck();
        setMove(PINK, ghost);
        break;
    case CHASE:
        portalCheck();
        setMove(PINK, ghost);
        break;
    case FRIGHTEN:
        portalCheck();
        setFrighten();
        break;
    case DEATH: setDeath(PINK, ghost);
        break;
    case LEAVE: leaveMove();
        break;
    case PEN: penMove();
        break;
    }
}

    int Ghost::getTile(float x, float y) {
        return map.mapMatrix[(int)floor(x)][(int)floor(y)];
    }

    int Ghost::getGhostX() {
        return (int)floor(ghostX);
    }
    int Ghost::getGhostY() {
        return (int)floor(ghostY);
    }

    int Ghost:: CheckAhead(int dir) {//1-up,2-down,3-right,4-left
        switch (dir) {
        case LEFT:
            return getTile(getGhostX() - 1, getGhostY());
        case UP:
            return getTile(getGhostX(), getGhostY() + 1);
        case RIGHT:
            return getTile(getGhostX() + 1, getGhostY());
        case DOWN:
            return getTile(getGhostX(), getGhostY() - 1);
        default:
            return getTile(getGhostX(), getGhostY());
        }
    }

    bool Ghost::isOnlyWall(int dir)
    {
        if (CheckAhead(dir) == 1) {
            return true;
        }

        return false;
    }

    bool Ghost::isWall(int dir)
    {
        if (CheckAhead(dir) == 1 || CheckAhead(dir) == 5) {
            return true;
        }

        return false;
    }

    bool  Ghost::isAtCenter() {
        return (int)(ghostY * 10.0f) % 10 == 0 && (int)(ghostX * 10.0f) % 10 == 0;
    }

    void  Ghost::cornerHandler() {//1-up,2-down,3-right,4-left
        if (isAtCenter()) { // Checks that it is completely on the cell
            if (isWall(gDirection)) { // Checks if it will continue moving in the current direction if it hits a wall
                if (!isWall(LEFT) && gDirection != RIGHT) { // Checks that its new direction won't cause it to hit a wall and that its direction won't change
                    gDirection = LEFT;
                }
                else if (!isWall(UP) && gDirection != DOWN) {
                    gDirection = UP;
                }
                else if (!isWall(RIGHT) && gDirection != LEFT) {
                    gDirection = RIGHT;
                }
                else {
                    gDirection = DOWN;
                }
            }
        }
    }

    int Ghost::checkAllDirections(float x, float y) {
        int exit = 0;
        if (!isWall(LEFT)) { // Check all directions and if it is not a wall increase the counter
            exit++;
        }
        if (!isWall(UP)) {
            exit++;
        }
        if (!isWall(RIGHT)) {
            exit++;
        }
        if (!isWall(DOWN)) {
            exit++;
        }

        return exit;
    }

    vector<int> Ghost::getTargetTile(colour ghostCol)
    {
        vector<int> v;
        switch (ghostState) {
        case CHASE:
            switch (ghostCol) {
            case RED:
                v.push_back(pacman.getPacmanX());
                v.push_back(pacman.getPacmanY());
                break;
            case BLUE:
                switch (pacman.pDirection) {
                case LEFT:
                    v.push_back(pacman.getPacmanX() - 2);
                    v.push_back(pacman.getPacmanY());
                    break;
                case UP:
                    v.push_back(pacman.getPacmanX());
                    v.push_back(pacman.getPacmanY() + 2);
                    break;
                case RIGHT:
                    v.push_back(pacman.getPacmanX() + 2);
                    v.push_back(pacman.getPacmanY());
                    break;
                case DOWN:
                    v.push_back(pacman.getPacmanX());
                    v.push_back(pacman.getPacmanY() - 2);
                    break;
                default:
                    v.push_back(pacman.getPacmanX());
                    v.push_back(pacman.getPacmanY());
                }
                break;
            case YELLOW:
                v.push_back(0);
                v.push_back(0);
                break;
            case PINK:
                switch (pacman.pDirection) {
                case LEFT:
                    v.push_back(pacman.getPacmanX() - 4);
                    v.push_back(pacman.getPacmanY());
                    break;
                case UP:
                    v.push_back(pacman.getPacmanX());
                    v.push_back(pacman.getPacmanY() + 4);
                    break;
                case RIGHT:
                    v.push_back(pacman.getPacmanX() + 4);
                    v.push_back(pacman.getPacmanY());
                    break;
                case DOWN:
                    v.push_back(pacman.getPacmanX());
                    v.push_back(pacman.getPacmanY() - 4);
                    break;
                default:
                    v.push_back(pacman.getPacmanX());
                    v.push_back(pacman.getPacmanY());
                }
                break;
            }
            break;
        case SCATTER:
            switch (ghostColour) {
            case RED:
                v.push_back(100);
                v.push_back(100);
                break;
            case BLUE:
                v.push_back(20);
                v.push_back(0);
                break;
            case YELLOW:
                v.push_back(0);
                v.push_back(0);
                break;
            case PINK:
                v.push_back(0);
                v.push_back(100);
                break;
            }
            break;
        case DEATH:
            v.push_back(13.5);
            v.push_back(19);
            break;
        }

        return v;
    }

    float Ghost::getDistance(int x, int y, colour ghostCol, Ghost ghost) {
        vector<int> v = getTargetTile(ghostCol);
        int targetX = v.at(0);
        int targetY = v.at(1);

        if (ghostCol == BLUE && ghostState == CHASE) {
            float tempX = ghost.getGhostX() - targetX;
            float tempY = ghost.getGhostY() - targetY;
            targetX = ghost.getGhostX() + 2 * tempX;
            targetY = ghost.getGhostY() + 2 * tempY;
        }

        return (sqrt(((targetX - x) * (targetX - x)) + ((targetY - y) * (targetY - y))));
    }


    void Ghost::moveTargetTile(colour colGhost, Ghost ghost)
    {
        float distance = 999.0f;
        if (checkAllDirections(ghostX, ghostY) >= 3 && isAtCenter()) {
            if (gDirection != DOWN && !isWall(UP)) {
                float d = getDistance(getGhostX(), getGhostY() + 1, colGhost, ghost);
                if (d < distance) {
                    tempDir = UP;
                    distance = d;
                }
            }
            if (gDirection != LEFT && !isWall(RIGHT)) {
                float d = getDistance(getGhostX() + 1, getGhostY(), colGhost, ghost);
                if (d < distance) {
                    tempDir = RIGHT;
                    distance = d;
                }
            }
            if (gDirection != RIGHT && !isWall(LEFT)) {
                float d = getDistance(getGhostX() - 1, getGhostY(), colGhost, ghost);
                if (d < distance) {
                    tempDir = LEFT;
                    distance = d;
                }
            }
            if (gDirection != UP && !isWall(DOWN)) {
                float d = getDistance(getGhostX(), getGhostY() - 1, colGhost, ghost);
                if (d < distance) {
                    tempDir = DOWN;
                    distance = d;
                }
            }
            gDirection = tempDir;
        }
        else {
            cornerHandler();
        }


        switch (gDirection) {
        case LEFT:
            ghostX -= speed;
            ghostY = round(ghostY);
            break;
        case UP:
            ghostY += speed;
            ghostX = round(ghostX);
            break;
        case RIGHT:
            ghostX += speed;
            ghostY = round(ghostY);
            break;
        case DOWN:
            ghostY -= speed;
            ghostX = round(ghostX);
            break;
        }
    }

    void Ghost::portalCheck()
    {
        switch (getTile(ghostX, ghostY)) {
        case 3: if (getGhostX() >= 26.5) {
            ghostX = 1.0f;
        }
              else {
            ghostX = 26.5;
        }
        }
    }

    void Ghost::penMove()
    {
        if (ghostY <= 15.5f && gDirection == DOWN) {
            if (ghostColour == PINK && ghostState == PEN) {
                ghostState = LEAVE;
            }
            else {
                gDirection = UP;
            }
        }
        else if (ghostY >= 16.5f && gDirection == UP) {
            gDirection = DOWN;
        }

        switch (gDirection) {
        case UP:
            ghostY += speed;
            break;
        case DOWN:
            ghostY -= speed;
            break;
        }
    }


    void Ghost::reset() {
        switch (ghostColour) { // Different settings depending on the colour of the ghost
        case RED:
            ghostX = 13.5f;
            ghostY = 19.0f;
            gDirection = LEFT;
            ghostState = SCATTER;
            break;
        case BLUE:
            ghostX = 11.5f;
            ghostY = 16.0f;
            gDirection = UP;
            if (ghostScore == 30) {
                ghostState = LEAVE;
            }
            else {
                ghostState = PEN;
            }
            break;
        case YELLOW:
            ghostX = 15.5f;
            ghostY = 16.0f;
            gDirection = UP;
            if (ghostScore == 80) {
                ghostState = LEAVE;
            }
            else {
                ghostState = PEN;
            }
            break;
        case PINK:
            ghostX = 13.5f;
            ghostY = 16.0f;
            gDirection = DOWN;
            ghostState = LEAVE;
            break;
        }
        eaten = false; // Upon reset no ghost has been eaten
    }

    /**
    * Method that allows ghosts to leave their starting location and enter the maze, no matter where they are.
    */
    void Ghost::leaveMove()
    {
        if (ghostX <= 13.55 && ghostX >= 13.45) { // Check if X coordinates are correct
            if (ghostY < 19.0f) { // move up and out of start location
                gDirection = UP;
            }
        }
        else { // If they have incorrect x coordinates
            if (ghostX < 13.5f) { // Determine if they need to move left or right
                gDirection = RIGHT;
            }
            else if (ghostX > 13.5f) {
                gDirection = LEFT;
            }
        }

        if (ghostY >= 19.0f) {
            gDirection = LEFT;
            if (isAtCenter()) {
                ghostState = SCATTER;
            }
        }

        if (ghostState == LEAVE) { // Since gDirection is changed on the last call to this method, they can still move 0.1f in a direction when they shouldn't, to avoid this you need to check gDirection
            switch (gDirection) {
            case LEFT:
                ghostX -= speed;
                break;
            case UP:
                ghostY += speed;
                break;
            case RIGHT:
                ghostX += speed;
                break;
            case DOWN:
                ghostY -= speed;
                break;
            }
        }

    }

    void Ghost::draw() {
        glPushMatrix();

        unsigned int ghost_tex;
        unsigned int ghost_eye_tex;

        if ( ghostState != FRIGHTEN && ghostState != DEATH) { // As long as the ghost is not in death or frighten mode, assign its relevant texture
            int tickDiv = gameTick % 20;
            if (tickDiv % 20 < 10) {
                switch (ghostColour) { // Check ghost colour in order to determine which tex file to use
                case RED:
                    ghost_tex = ghost_r_0_tex;
                    break;
                case BLUE:
                    ghost_tex = ghost_b_0_tex;
                    break;
                case PINK:
                    ghost_tex = ghost_p_0_tex;
                    break;
                case YELLOW:
                    ghost_tex = ghost_y_0_tex;
                    break;
                }
            }
            else {
                switch (ghostColour) {
                case RED:
                    ghost_tex = ghost_r_1_tex;
                    break;
                case BLUE:
                    ghost_tex = ghost_b_1_tex;
                    break;
                case PINK:
                    ghost_tex = ghost_p_1_tex;
                    break;
                case YELLOW:
                    ghost_tex = ghost_y_1_tex;
                    break;
                }
            }

            switch (gDirection) { // Eye position/tex file is dependent on direction the ghost is moving
            case LEFT:
                ghost_eye_tex = eye_left_tex;
                break;
            case UP:
                ghost_eye_tex = eye_up_tex;
                break;
            case RIGHT:
                ghost_eye_tex = eye_right_tex;
                break;
            case DOWN:
                ghost_eye_tex = eye_down_tex;
                break;
            }
        }
        else if (ghostState == FRIGHTEN) { // If the ghost is frightened assign its relevant texture
            int tickDiv = gameTick % 40;
            if (tickDiv % 40 < 10) {
                ghost_tex = ghost_scared_0_tex;
            }
            else if (tickDiv % 40 < 20) {
                ghost_tex = ghost_scared_1_tex;
            }
            else if (tickDiv % 40 < 30) {
                ghost_tex = ghost_scared_2_tex;
            }
            else {
                ghost_tex = ghost_scared_3_tex;
            }
        }
        else  if (ghostState == DEATH) {
            switch (gDirection) { // Eye position/tex file is dependent on direction the ghost is moving
            case LEFT:
                ghost_eye_tex = eye_left_tex;
                break;
            case UP:
                ghost_eye_tex = eye_up_tex;
                break;
            case RIGHT:
                ghost_eye_tex = eye_right_tex;
                break;
            case DOWN:
                ghost_eye_tex = eye_down_tex;
                break;
            }
        }

        glTranslatef(ghostX, ghostY, 1.0f);

        if (ghostState != DEATH) { // If the ghost is not dead then draw its body texture
            drawTex(ghost_tex, 2, 2, 0);
        }

        if (ghostState != FRIGHTEN) { // If the ghost is not frightened then draw the eye texture
            drawTex(ghost_eye_tex, 2, 2, 0);
        }

        glPopMatrix();

    }