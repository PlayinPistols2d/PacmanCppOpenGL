#include "glew.h"
#include "texture.h"
#include <string>
#include <sstream>
#include <fstream>
using namespace std;


extern int Score;
extern int lives;
extern bool paused;
/**
 * Takes in the current score and draws it to the screen
 * @param scorePrint the current score in the game
 */
void drawScore(int scorePrint)
{
    glPushMatrix();
    glTranslatef(30, 27, 1);
    drawTex(score_tex, 8, 3, 0); // Draw the score_tex which is the word "SCORE"
    glTranslatef(6, -4, 0);
    string str = to_string(scorePrint); // Convert the current score to a string to enable iteration through number
    for (int i = str.length() - 1; i >= 0; i--) { // Iterate through the number and draw the correct digit
        switch (str[i]) {
        case '0':
            drawTex(num_0_tex, 2, 2, 0);
            break;
        case '1':
            drawTex(num_1_tex, 2, 2, 0);
            break;
        case '2':
            drawTex(num_2_tex, 2, 2, 0);
            break;
        case '3':
            drawTex(num_3_tex, 2, 2, 0);
            break;
        case '4':
            drawTex(num_4_tex, 2, 2, 0);
            break;
        case '5':
            drawTex(num_5_tex, 2, 2, 0);
            break;
        case '6':
            drawTex(num_6_tex, 2, 2, 0);
            break;
        case '7':
            drawTex(num_7_tex, 2, 2, 0);
            break;
        case '8':
            drawTex(num_8_tex, 2, 2, 0);
            break;
        case '9':
            drawTex(num_9_tex, 2, 2, 0);
            break;
        }
       glTranslatef(-2, 0, 0); // Translate when the ^10 index is increased by 1 to ensure number reads correctly
    }
    glPopMatrix();
}

/**
 * Draws the current number of lives to the screen
 * @param liveCount number of lives pacman has left in current game
 */
void drawLives(int liveCount)
{
    glPushMatrix();
    glTranslatef(30, -1, 1);
    for (int i = 0; i < liveCount - 1; i++) { // Draw as many as icons as pacman has lives left
        drawTex(life_tex, 12, 12, 0);
        glTranslatef(-3,0, 0);
    }
    glPopMatrix();
}

/**
 * Method to return the highscore from high_score.txt file. If the file doesnt exist it is created and the highscore is set to 0
 * @return the current high score
 */
int getHighScore()
{
    int highScore;
    fstream scoreFile("high_score.txt"); // Open a stream to the high score file
    if (scoreFile.good()) { // If the file exists get the value from inside
        scoreFile >> highScore;
        scoreFile.close(); // Close the stream
        return highScore; // Return the score
    }
    else { // If the file does not exist
        ofstream highScoreFile("high_score.txt"); // Create the file
        highScoreFile << 0; // Pass in the highscore of 0
        scoreFile.close(); // close the streams
        highScoreFile.close();
    }
    return 0; // Return 0 as the file hadnt previously existed
}

/**
 * Once the game is over the final score is compared to the current highscore and determines whether to overwrite the highscore wit the new
 * @param score the score at the end of the current game
 */
void setHighScore(int score)
{
    int currentHigh = getHighScore(); // Store the current high score
    if (currentHigh < score) { // Only update the high score if the current score is higher than the previous
        ofstream highFile("high_score.txt"); // Open the stream for the highscore file
        highFile.clear(); // clear the stream
        highFile << score; // write the new high score to the stream
        highFile.close(); // close the stream
    }
}

/**
 * Draws the "HIGH" text and the vale of the current highscore to the correct area of the screen
 */
void drawHigh()
{
    glPushMatrix();
    glTranslatef(42, 27, 1); // Draw the high_tex which is the word "HIGH"
    drawTex(high_tex, 8, 2, 0);
    string str = to_string(getHighScore()); // Convert the current score to a string to enable iteration through number
    glTranslatef(6, -4, 0);
    for (int i = str.length() - 1; i >= 0; i--) { // Iterate through the number and draw the correct digit
        switch (str[i]) {
        case '0':
            drawTex(num_0_tex, 2, 2, 0);
            break;
        case '1':
            drawTex(num_1_tex, 2, 2, 0);
            break;
        case '2':
            drawTex(num_2_tex, 2, 2, 0);
            break;
        case '3':
            drawTex(num_3_tex, 2, 2, 0);
            break;
        case '4':
            drawTex(num_4_tex, 2, 2, 0);
            break;
        case '5':
            drawTex(num_5_tex, 2, 2, 0);
            break;
        case '6':
            drawTex(num_6_tex, 2, 2, 0);
            break;
        case '7':
            drawTex(num_7_tex, 2, 2, 0);
            break;
        case '8':
            drawTex(num_8_tex, 2, 2, 0);
            break;
        case '9':
            drawTex(num_9_tex, 2, 2, 0);
            break;
        }
       // Translate when the ^10 index is increased by 1 to ensure number reads correctly
        glTranslatef(-2, 0, 0);
    }
    glPopMatrix();
}


int getPreviousScore()
{
    int previousScore;
    fstream scoreFile("previous_score.txt"); // Open a stream to the high score file
    if (scoreFile.good()) { // If the file exists get the value from inside
        scoreFile >> previousScore;
        scoreFile.close(); // Close the stream
        return previousScore; // Return the score
    }
    else { // If the file does not exist
        ofstream previousScoreFile("previous_score.txt"); // Create the file
        previousScoreFile << 0; // Pass in the highscore of 0
        scoreFile.close(); // close the streams
        previousScoreFile.close();
    }
    return 0; // Return 0 as the file hadnt previously existed
}

void setPreviousScore(int current_score)
{
    int previousScore = getPreviousScore(); // Store the current high score
     // Only update the high score if the current score is higher than the previous
        ofstream previousFile("previous_score.txt"); // Open the stream for the highscore file
        previousFile.clear(); // clear the stream
        previousFile << current_score; // write the new high score to the stream
        previousFile.close(); // close the stream   
}

void drawPrevious()
{
    glPushMatrix();
    glTranslatef(42, 7.5, 1); // Draw the high_tex which is the word "HIGH"
    drawTex(previous_tex, 8, 3, 0);
// Translate to below "HIGH" ready to draw numbers
    string str = to_string(getPreviousScore()); // Convert the current score to a string to enable iteration through number
    glTranslatef(6, -4, 0);
    for (int i = str.length() - 1; i >= 0; i--) { // Iterate through the number and draw the correct digit
        switch (str[i]) {
        case '0':
            drawTex(num_0_tex, 2, 2, 0);
            break;
        case '1':
            drawTex(num_1_tex, 2, 2, 0);
            break;
        case '2':
            drawTex(num_2_tex, 2, 2, 0);
            break;
        case '3':
            drawTex(num_3_tex, 2, 2, 0);
            break;
        case '4':
            drawTex(num_4_tex, 2, 2, 0);
            break;
        case '5':
            drawTex(num_5_tex, 2, 2, 0);
            break;
        case '6':
            drawTex(num_6_tex, 2, 2, 0);
            break;
        case '7':
            drawTex(num_7_tex, 2, 2, 0);
            break;
        case '8':
            drawTex(num_8_tex, 2, 2, 0);
            break;
        case '9':
            drawTex(num_9_tex, 2, 2, 0);
            break;
        }
        // translateToMazeCoords(-1, 0); // Translate when the ^10 index is increased by 1 to ensure number reads correctly
        glTranslatef(-2, 0, 0);
    }
    glPopMatrix();
}
/**
 * Draw the ready text at the beginning of the game
 */
void drawReady()
{
    glPushMatrix();
    glTranslatef(9.6, 13, 1);
    drawTex(ready_tex, 10, 3, 0);
    glPopMatrix();
}

/**
 * Draw the game over text at the end of the game
 */
void drawGameOver()
{
    glPushMatrix();
    glTranslatef(9.6, 19, 1);
    drawTex(gameover_tex, 10, 3, 0);
    glPopMatrix();
}

void drawYouWon()
{
    glPushMatrix();
    glTranslatef(9.6, 19, 1);
    drawTex(youwon_tex, 10, 3, 0);
    glPopMatrix();
}

void drawLivesText()
{
    glPushMatrix();
    glTranslatef(30, 7.5, 1);
    drawTex(lives_tex, 10, 3, 0);
    glPopMatrix();
}

void drawPauseHint()
{
    glPushMatrix();
    glTranslatef(32.3, 17, 1);
    drawTex(pausehint_tex,16, 3, 0);
    glPopMatrix();
}

void drawUnpauseHint()
{
    glPushMatrix();
    glTranslatef(31.5, 17, 1);
    drawTex(unpausehint_tex, 18, 3, 0);
    glPopMatrix();
}

void drawQuitHint()
{
    glPushMatrix();
    glTranslatef(31.5, 13, 1);
    drawTex(quithint_tex, 18, 3, 0);
    glPopMatrix();
}

void drawRestartHint()
{
    glPushMatrix();
    glTranslatef(5.6, 13, 1);
    drawTex(restarthint_tex, 18, 3, 0);
    glPopMatrix();
}

void drawPause()
{
    glPushMatrix();
    glTranslatef(9.6, 13, 1);
    drawTex(paused_tex, 10, 3, 1);
    glPopMatrix();
}

void drawInterface()
{
    drawScore(Score);
    drawHigh();
    drawPrevious();
    drawLivesText();
    drawLives(lives);
    drawQuitHint();
    if (!paused)
    {
        drawPauseHint();
    }
}
