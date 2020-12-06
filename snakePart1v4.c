#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h> 

int trophyPosX, trophyPosY, randPosX, randPosY, randBonus;
char trophy[100];
int trophyTimeLimit = 0;
clock_t trophyStartTime;
bool trophyCreate = true;
bool trophyObtained = false;
void trophySpawn();

bool gameWon = false;
bool gameEnd = false;
void printEndScreen();
int randInt(int, int);

int main(int ac, char *av[])
{
    initscr();
    clear();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);

    /* Yaw - Prints the top and then the side borders*/
    for (int i = 0; i <= (COLS - 1); i++)
    {
        move(0,i);
        addstr("#");

        move(LINES-1,i);
        addstr("#");
    }
    for (int i = 0; i < (LINES - 1); i++)
    {
        move(i,0);
        addstr("#");

        move(i,COLS-1);
        addstr("#");
    }
    int halfPerim = COLS+LINES;

    /* Yaw - Initialize snake direction, size, and position */
    char snakeUnit[] = "o";
    char removeUnit[] = " ";
    int dirX, dirY;
    char *dir = " right ";
    int printPosX = COLS/2;
    int printPosY = LINES/2;

    /* Adam - Snake Size , Unit counter, Tail unit index, Tail coordinate arrays, and randomly choose initial direction */
    int snakeSize = 4;
    int snakeUnitCount = 0;
    int tailUnitIndex = 0;
    int tailPosX[halfPerim+1], tailPosY[halfPerim+1];
    int ranDir = randInt(0, 3);
    switch (ranDir) 
    {
        case 0:
            dirX = 1;
            dirY = 0;
            dir = " right ";
            break;

        case 1:
            dirX = 0;
            dirY = -1;
            dir = " up    ";
            break;

        case 2:
            dirX = -1;
            dirY = 0;
            dir = " left  ";
            break;

        case 3:
            dirX = 0;
            dirY = 1;
            dir = " down  ";
            break;

        default:
            break;
    }

    /*Yaw - Main game loop */
    while(!gameEnd)
    {
        /* Adam - Show column count, line count, and half the perimeter for debug
        char debugPerim[100];
        sprintf(debugPerim, "cols=%d lines=%d perim/2=%d", COLS,LINES,COLS+LINES);
        move(1, 1);
        addstr(debugPerim);*/
        /* Adam - Create a new trophy if the time runs out or the snake eats it */
        if (trophyCreate) 
        {
            trophySpawn();
        }

        /* Adam - Update tail coord arrays */
        tailPosX[tailUnitIndex] = printPosX;
        tailPosY[tailUnitIndex] = printPosY;

        /* Yaw - Prints one unit of the snake*/
        usleep(100000);
        move(printPosY,printPosX);
        addstr(snakeUnit);
        move(LINES-1,COLS-1);
        refresh();
        printPosX += dirX;
        printPosY += dirY;

        /* Adam - increments the count of snake units and the tail index and then checks if the head of the snake colliding with anything */
        snakeUnitCount++;
        if (tailUnitIndex<snakeSize-1) 
        {
            tailUnitIndex++;
        }
        if(tailPosX[snakeSize-1] == trophyPosX && tailPosY[snakeSize-1] == trophyPosY)
        {
            trophyObtained = true;
            snakeSize+=randBonus;
            //move(2, 1);
            //addstr("trophyObtained = true ");
        } 
        if (tailPosX[snakeSize-1] == 0 || tailPosX[snakeSize-1] == COLS-1)
        {
            gameEnd = true;
        }
        else if (tailPosY[snakeSize-1] == 0 || tailPosY[snakeSize-1] == LINES-1) 
        {
            gameEnd = true;
        }
        else
        {
            for(int i = 0; i<snakeSize-1; i++)
            {
                if(tailPosX[snakeSize-1] == tailPosX[i] && tailPosY[snakeSize-1] == tailPosY[i])
                {
                    gameEnd = true;
                }
            }
        }

        /* Adam - Debug code that shows where the trophy and each unit of the snake is 
        move(1, COLS/2);
        char debugTrLoc[100];
        sprintf(debugTrLoc, "trophy position: (%d,%d)                           ", trophyPosX,trophyPosY);
        addstr(debugTrLoc); 
        for (int i = 0; i<snakeSize; i++) 
        {
            move(i+1, 3*COLS/4);
            char debugString[100];
            sprintf(debugString, "tail position %d[%d]: (%d,%d)                           ", i, tailUnitIndex, tailPosX[i],tailPosY[i]);
            addstr(debugString);                        
        }*/
        
        /* Adam - Collects keyboard input and determines direction. Does not allow the snake to reverse. */
        timeout(50);
        int userInput=getch();
        if((userInput == 'w' || userInput == KEY_UP) && dirY > 0)
        {
            userInput = KEY_DOWN;
        }
        else if ((userInput == 's' || userInput == KEY_DOWN) && dirY < 0) 
        {
            userInput = KEY_UP;
        }
        else if ((userInput == 'a' || userInput == KEY_LEFT) && dirX > 0) 
        {
            userInput = KEY_RIGHT;
        }
        else if ((userInput == 'd' || userInput == KEY_RIGHT) && dirX < 0) 
        {
            userInput = KEY_LEFT;
        }
        switch (userInput) 
        {
            case 'w':
            case KEY_UP:
                dirX = 0;
                dirY = -1;
                dir = " up    ";
                break;

            case 's':
            case KEY_DOWN:
                dirX = 0;
                dirY = 1;
                dir = " down  ";
                break;

            case 'a':
            case KEY_LEFT:
                dirX = -1;
                dirY = 0;
                dir = " left  ";
                break;

            case 'd':
            case KEY_RIGHT:
                dirX = 1;
                dirY = 0;
                dir = " right ";
                break;

            default:
                break;
        }

        /* Adam - Removes tail of snake when too many snakeUnits are printed */
        if (snakeUnitCount>=snakeSize)
        {
            move(tailPosY[0],tailPosX[0]);
            addstr(removeUnit);
            refresh();
            snakeUnitCount--;
            tailUnitIndex=snakeSize-1;
            /* Adam - shifts the tail position array data left */
            for (int i = 0; i<snakeSize-1; i++) 
            {
                tailPosX[i] = tailPosX[i+1]; 
                tailPosY[i] = tailPosY[i+1];                         
            }
        }
        /* Adam - Calculates approximately how many seconds have passed since the new trophy spawn */
        int trophyTime = (int)((clock()-trophyStartTime)/1500)-trophyTimeLimit;
        if(trophyTime>=trophyTimeLimit || trophyObtained)
        {
            trophyCreate = true;
            //move(1, 1);
            //addstr("trophy = true ");
        }

        if(snakeSize>halfPerim)
        {
            gameWon = true;
            gameEnd = true;
        }
        if(gameEnd)
        {
            printEndScreen(gameWon);
        }
        usleep(300000/snakeSize);
    }
    return 0;
}

int randInt(int lowLim, int highLim)
{
    srand(time(0));
    return (rand() % (highLim - lowLim + 1)) + lowLim;
}

void trophySpawn()
{
    trophyObtained = false;
    move(trophyPosY, trophyPosX);
    addstr(" ");
    usleep(10000);
    randPosX = randInt(2, COLS-2);
    randPosY = randInt(2, LINES-2);
    randBonus = randInt(1, 9);
    trophyTimeLimit = randInt(1, 9);
    sprintf(trophy, "%d", randBonus);
    move(randPosY, randPosX);
    addstr(trophy);
    refresh();
    trophyStartTime = clock();
    trophyPosX = randPosX;
    trophyPosY = randPosY;
    trophyCreate = false;
    //move(1, 1);
    //addstr("trophy = false");
}

void printEndScreen()
{
    for(int i = 0; i<LINES; i++)
    {
        for(int j = 0; j<COLS; j++)
        {
            move(i,j);
            if(gameWon)
            {
                addstr("*");
            }
            else 
            {
                addstr("x");
            }
            refresh();
            usleep(500);
        }
    }
    for(int i = LINES/4; i<3*LINES/4; i++)
    {
        for(int j = COLS/4; j<3*COLS/4; j++)
        {
            move(i,j);
            addstr(" ");
            refresh();
            usleep(500);
        }
    }
    if(gameWon)
    {
        move(LINES/2, COLS/2-4);
        addstr("YOU WIN!");
    }
    else 
    {
        move(LINES/2, COLS/2-4);
        addstr("YOU LOSE");
    }
    refresh();
    sleep(5);
}