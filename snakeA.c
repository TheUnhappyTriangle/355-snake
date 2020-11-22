#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <curses.h>


int main(int ac, char *av[]) {
    initscr();
    clear();
    curs_set(0);
    noecho();
    keypad(stdscr,TRUE);
    move(0,0);
    addstr("Press any key...");
    while(1)
    {
        int ch=getch();
        char buf[100];
        move(0,0);

        switch(ch) 
        {
            case 'w':
            case KEY_UP:
                addstr("The snake is moving up                           ");
                break;

            case 's':
            case KEY_DOWN:
                addstr("The snake is moving down                           ");
                break;

            case 'a':
            case KEY_LEFT:
                addstr("The snake is moving left                           ");
                break;

            case 'd':
            case KEY_RIGHT:
                addstr("The snake is moving right                           ");
                break;

            case 10:
            case KEY_ENTER:
                addstr("The snake is paused                           ");
                break;

            case 'x':
            case 'X':
                endwin();
                return 0;
            default:
                sprintf(buf,"You pressed a \"%c\" key, ASCII code %d...", (char)ch, ch);
                addstr(buf);
                break;
        }
        refresh();
    }
}