#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <stdlib.h>

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
    int dirX = 1;
    int dirY = 0;
    char *dir = " right ";
    int printPosX = COLS/2;
    int printPosY = LINES/2;

    /* Adam - Snake Size and Unit counter */
    int snakeSize = 4;
    int snakeUnitCount = 0;
    int tailUnitIndex = 0;

    /*Yaw - Main game loop */
    while(1)
    {
        /* Adam - Initialize tail coord array and update them */
        int tailPosX[snakeSize], tailPosY[snakeSize];
        tailPosX[tailUnitIndex] = printPosX;
        tailPosY[tailUnitIndex] = printPosY;

        /* Yaw - Prints one unit of the snake*/
        //usleep(100000);
        //sleep(1);
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

        for (int i = 0; i<snakeSize; i++) 
        {
            move(i+1, 3*COLS/4);
            char debugString[100];
            sprintf(debugString, "tail position %d[%d]: (%d,%d)                              ", i, tailUnitIndex, tailPosX[i],tailPosY[i]);
            addstr(debugString);                        
        }
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

        /* Adam - Removes tail of snake*/
        if (snakeUnitCount>=snakeSize)
        {
            move(1, 1);
            addstr("hello");
            move(tailPosY[0],tailPosX[0]);
            addstr(removeUnit);
            refresh();
            snakeUnitCount--;
            tailUnitIndex=snakeSize-1;
            for (int i = 0; i<snakeSize-1; i++) 
            {
                tailPosX[i] = tailPosX[i+1]; 
                tailPosY[i] = tailPosY[i+1];                         
            }
        }
        
        move(1, 1);
        addstr("00000");
        usleep(100000);
    }
    return 0;
}