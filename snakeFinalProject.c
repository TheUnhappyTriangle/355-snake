#include <stdbool.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <stdlib.h>
#include <time.h> 

/* Adam - Global trophy variables and declaration of trophySpawn() function. */
int trophyPosX, trophyPosY, randPosX, randPosY, randBonus, trophyTimeLimit;
char trophy[100];
clock_t trophyStartTime;
bool trophyCreate = true;
bool trophyObtained = false;
void trophySpawn();

/* Adam - Global game end variables and declaration of printEndScreen() function. */
bool gameWon = false;
bool gameEnd = false;
void printEndScreen();

/* Adam - Declaration of randInt(int, int) */
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

    /* Yaw - Initialize snake direction, size, and position */
    char snakeUnit[] = "o";
    char removeUnit[] = " ";
    int dirX, dirY;
    char *dir = " right ";
    int printPosX = COLS/2;
    int printPosY = LINES/2;

    /* Adam - Snake Size, Snake Unit counter, head unit index, the goal snake size of half the perimeter, 
              Tail coordinate arrays, and randomly choose initial direction */
    int snakeSize = 4;
    int snakeUnitCount = 0;
    int headUnitIndex = 0;
    int halfPerim = COLS+LINES;
    int tailPosX[halfPerim+1], tailPosY[halfPerim+1];
    int snakeHeadX, snakeHeadY;
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
        /* Adam - Create a new trophy if the time runs out or the snake eats it */
        if (trophyCreate) 
        {
            trophySpawn();
        }

        /* Adam - Update tail coord arrays and the snake head coords */
        tailPosX[headUnitIndex] = printPosX;
        tailPosY[headUnitIndex] = printPosY;
        snakeHeadX = tailPosX[headUnitIndex];
        snakeHeadY = tailPosY[headUnitIndex];

        /* Yaw - Prints one unit of the snake and then updates the print position based on the player determined direction. */
        usleep(300000/snakeSize);
        move(printPosY,printPosX);
        addstr(snakeUnit);
        move(LINES-1,COLS-1);
        refresh();
        printPosX += dirX;
        printPosY += dirY;

        /* Adam - Increments the count of snake units when one is printed and the head index if necessary. */
        snakeUnitCount++;
        if (headUnitIndex<snakeSize-1) 
        {
            headUnitIndex++;
        }

        /* Adam - This code block checks if the head of the snake colliding with either a trophy, the borders, or itself. 
                  The snake grows when colliding with the trophy and sets trophyObtained to true which triggers a new trophy
                  spawn. When the snake collides with the border or itself, gameEnd is set to true, which later triggers the
                  end screen to be printed. */
        if(snakeHeadX == trophyPosX && snakeHeadY == trophyPosY)
        {
            trophyObtained = true;
            snakeSize+=randBonus;
        } 
        else if ((snakeHeadX == 0 || snakeHeadX == COLS-1) && snakeUnitCount>=snakeSize)
        {
            gameEnd = true;
            move(1, 1);
            addstr("You ran into the left/right border!");
            refresh();
            usleep(1500000);
        }
        else if ((snakeHeadY == 0 || snakeHeadY == LINES-1) && snakeUnitCount>=snakeSize) 
        {
            gameEnd = true;
            move(2, 1);
            addstr("You ran into the upper/lower border!");
            refresh();
            usleep(1500000);
        }
        else
        {
            if(snakeUnitCount>=snakeSize)
            {
                for(int i = 0; i<snakeSize-2; i++)
                {
                    if(snakeHeadX == tailPosX[i] && snakeHeadY == tailPosY[i])
                    {
                        gameEnd = true;
                        move(3, 1);
                        addstr("You ran into yourself!");
                        refresh();
                        usleep(1500000);
                    }
                }
            }
        }
        
        /* Adam - This code block collects the keyboard input and determines direction. 
                  The if-else-if statements prevent the snake from reversing. */
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
                usleep(100);
                break;

            case 's':
            case KEY_DOWN:
                dirX = 0;
                dirY = 1;
                dir = " down  ";
                usleep(100);
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

        /* Adam - This if statement removes the tail end of the snake when too many snakeUnits are printed.
                  This should run each game loop unless the snake is growing (either at start
                  or after having obtained a trophy). */
        if (snakeUnitCount>=snakeSize)
        {
            move(tailPosY[0],tailPosX[0]);
            addstr(removeUnit);
            refresh();
            snakeUnitCount--;
            headUnitIndex=snakeSize-1;
            /* Adam - This for loop shifts the tail position array data left to reflect the removal of the extra unit. */
            for (int i = 0; i<snakeSize-1; i++) 
            {
                tailPosX[i] = tailPosX[i+1]; 
                tailPosY[i] = tailPosY[i+1];                         
            }
        }
        
        /* Adam - Calculates how many seconds have passed since the new trophy spawn
                  and then sets trophyCreate to true if the time limit has been met
                  or if the trophy was obtained by the player.*/
        int trophyTime = (int)((clock()-trophyStartTime)/1500)-trophyTimeLimit;
        if(trophyTime>=trophyTimeLimit || trophyObtained)
        {
            trophyCreate = true;
        }

        /* Adam - Checks if the requirements to end the game have been met. */
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

/* Adam - randInt(int, int) returns a random integer between the two input
          integers. */
int randInt(int lowLim, int highLim)
{
    srand(time(0));
    return (rand() % (highLim - lowLim + 1)) + lowLim;
}

/* Adam - trophySpawn() removes the current trophy from the screen and then
          determines where the next trophy should be printed, what the bonus
          should be, and how long it should stay on the the screen without
          being obtained by the player.*/
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
}

/* Adam - Prints the game over screen when the game ends. This function is triggered 
          in the main loop when gameEnd is true and it uses the bool gameWon to decide
          which end screen to display. */
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
            usleep(100);
        }
    }
    for(int i = LINES/4; i<3*LINES/4; i++)
    {
        for(int j = COLS/4; j<3*COLS/4; j++)
        {
            move(i,j);
            addstr(" ");
            refresh();
            usleep(100);
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
    sleep(120);
}