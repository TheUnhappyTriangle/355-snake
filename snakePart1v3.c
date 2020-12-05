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
int randInt(int, int);
void trophySpawn();

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

    /* Yaw - Initialize snake direction, size, and position */
    char snakeUnit[] = "o";
    char removeUnit[] = " ";
    int dirX, dirY;
    char *dir = " right ";
    int printPosX = COLS/2;
    int printPosY = LINES/2;

    /* Adam - Snake Size, Unit counter, Tail unit index, and randomly choose initial direction */
    int snakeSize = 4;
    int snakeUnitCount = 0;
    int tailUnitIndex = 0;
    //int trophyTimeLimit = 0;
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
    int tailPosX[100], tailPosY[100];
    /*Yaw - Main game loop */
    while(1)
    {
        if (trophyCreate) 
        {
            trophySpawn();
        }
        /* Adam - Initialize tail coord array and update them */
        
        tailPosX[tailUnitIndex] = printPosX;
        tailPosY[tailUnitIndex] = printPosY;

        /* Yaw - Prints one unit of the snake*/
        usleep(100000);
        move(printPosY,printPosX);
        addstr(snakeUnit);
        move(LINES-1,COLS-1);
        refresh();
        snakeUnitCount++;
        if (tailUnitIndex<snakeSize-1) 
        {
            tailUnitIndex++;
        }
        printPosX += dirX;
        printPosY += dirY;
        if(tailPosX[snakeSize-1] == trophyPosX && tailPosY[snakeSize-1] == trophyPosY)
        {
            trophyObtained = true;
            snakeSize+=randBonus;
            //move(2, 1);
            //addstr("trophyObtained = true ");
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
        } */
        
        /* Adam - Collects keyboard input and determines direction*/
        timeout(50);
        int ch=getch();
        switch (ch) 
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