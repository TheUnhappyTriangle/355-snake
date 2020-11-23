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
    /* Yaw */
    //print top/bottom border
    for (int i = 0; i <= (COLS - 1); i++)
    {
        move(0,i);
        addstr("#");

        move(LINES-1,i);
        addstr("#");
    }
    //print side borders
    for (int i = 0; i < (LINES - 1); i++)
    {
        move(i,0);
        addstr("#");

        move(i,COLS-1);
        addstr("#");
    }
    /* Yaw */
    char snake[] = "ooooo";
    char sm[] = "     ";
    int dirX = 1;
    int dirY = 0;
    int posX = COLS/2;
    int posY = LINES/2;
    while(1)
    {
        //snake placement, animation
        move(posY,posX);
        addstr(snake);
        move(LINES-1,COLS-1);
        refresh();
        usleep(100000);
        move(posY,posX);
        addstr(sm);
        posX += dirX;
        posY += dirY;
            
        /* Adam */
        //cbreak();
        timeout(5);
        int ch=getch();
        switch (ch) 
        {
            case 'w':
            case KEY_UP:
                dirX = 0;
                dirY = -1;
                break;

            case 's':
            case KEY_DOWN:
                dirX = 0;
                dirY = 1;
                break;

            case 'a':
            case KEY_LEFT:
                dirX = -1;
                dirY = 0;
                break;

            case 'd':
            case KEY_RIGHT:
                dirX = 1;
                dirY = 0;
                break;

            default:
                break;
        }
    }

    return 0;
}