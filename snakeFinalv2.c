#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <stdlib.h>

void snakeLoop();

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

    /* Yaw - Initialize snake direction and position */
    char snake[] = "o";
    char sm[] = " ";
    int dirX = 1;
    int dirY = 0;
    char *dir = " right ";
    int posX = COLS/2;
    int posY = LINES/2;

    /* Adam - Initialize important loop variables*/
    int snakeSize = 5;
    int tailX[snakeSize], tailY[snakeSize];
    tailX[0] = posX;
    tailY[0] = posY;

    /*Yaw - Main game loop */
    while(1)
    {
        /* Adam - Tells what direction for debug*/
        //move(0,0);
        //addstr(dir);
        /* Adam - Handles each unit of the snake*/
        for(int i=0; i<snakeSize; i++)
        {
            usleep(2000);
            /* Adam - Says when loop starts for debug*/
            //move(0, COLS/2);
            //addstr("loop start");
            /* Adam - Updates tail coordinate arrays*/
            tailX[i] = posX;
            tailY[i] = posY;

            /* Yaw - Prints one unit of the snake*/
            move(posY,posX);
            addstr(snake);
            move(LINES-1,COLS-1);
            refresh();
            posX += dirX;
            posY += dirY;

            /* Adam - Collects keyboard input and determines direction*/
            timeout(5);
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
        }
        /* Adam - Says when loop stops for debug*/
        //move(0, COLS/2);
        //addstr("loop stop ");
        /* Adam - Removes tail of snake*/
        for (int i = 0; i<snakeSize; i++) 
        {
            usleep(2000);
            move(tailY[i],tailX[i]);
            addstr(sm);
        }
        usleep(300000);
    }
    return 0;
}