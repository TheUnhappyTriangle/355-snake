#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <stdlib.h>

int main(int ac, char *av[])
{
    char snake[] = "o";
    char sm[] = " ";
    int dir = +1;
    int pos =2;

    initscr();
        //clear screen
        clear();

        //hide input printing
        noecho();

        //hide cursor
        curs_set(0);
        
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

        while(1)
        {
            //snake placement, animation
            move(LINES-3,pos);
            addstr(snake);
            move(LINES-1,COLS-1);
            refresh();
            sleep(1);
            move(LINES-3,pos);
            addstr(sm);
            pos +=dir;
            
            //moves snake
            if(pos>=COLS-2)
                dir=-1;
            if(pos<=1)
                dir=+1;
        }

    return 0;
}