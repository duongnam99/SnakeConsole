#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>

typedef enum STATUS STATUS;
typedef struct COORDINATES COORDINATES;
typedef struct SNAKE SNAKE;

srand(time(NULL));

enum STATUS
{
    UP, DOWN, RIGHT, LEFT, STOP
};

struct COORDINATES
{
    int x, y;
};

struct SNAKE
{
    int len;
    COORDINATES tail[200];
    STATUS status;
};

void MODE(int active);
void ABOUT();

// graph functions

//CursorVisibility = 0: hide cursor
//CursorVisibility = 1: show cursor
void ShowCur(int CursorVisibility)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor = {1, CursorVisibility};
    SetConsoleCursorInfo(handle, &cursor);
}

void textcolor(int x)
{
    HANDLE color;
    color = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(color, x);
}

void gotoxy(int x, int y)
{
    HANDLE hConsoleOutput;
    COORD Cursor_an_Pos = {x - 1, y - 1};
    hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void drawBackground(int x, int y, int col, int row, int color)
{
    int r;
    textcolor(color);
    for (r = 0; r < row; r++)
    {
        gotoxy(x, y + r);
        printf("%*c", col, ' ');

    }
}

void drawMenu(int active)
{
    //draw background
    drawBackground(38, 3, 49, 19, 34);

    int i, x = 9;

    //draw title
    textcolor(11);
    gotoxy(55, 4);
    printf("               ");
    gotoxy(55, 5);
    printf(" SNAKE-CONSOLE ");
    gotoxy(55, 6);
    printf("               ");

    //draw items
    textcolor(15);
    for (i = 0; i < 5; i++)
    {
        if (i + 1 == active)
        {
            textcolor(12);
            gotoxy(48, x);
            switch (i)
            {
                case 0:
                    printf("           P L A Y           ");
                    break;
                case 1:
                    printf("           M O D E           ");
                    break;
                case 2:
                    printf("          S P E E D          ");
                    break;
                case 3:
                    printf("      H I G H S C O R E      ");
                    break;
                case 4:
                    printf("          A B O U T          ");
                    break;
            }
            x += 2;
            textcolor(15);
            continue;
        }
        gotoxy(48, x);
        switch (i)
        {
            case 0:
                printf("            PLAY             ");
                break;
            case 1:
                printf("            MODE             ");
                break;
            case 2:
                printf("            SPEED            ");
                break;
            case 3:
                printf("          HIGH SCORE         ");
                break;
            case 4:
                printf("            ABOUT            ");
                break;
        }
        x += 2;
    }

    //draw copyright
    textcolor(14);
    gotoxy(40, 20);
    printf(" COPYRIGHT 2017 Gr^k-T. ALL RIGHTS RESERVED. ");
}

//end graph functions

//menu process
char readKeyMenu()
{
    while (1)
    {
        if (kbhit())
        {
            char ch = getch();
            if (ch == 'w' || ch == 'W')
                return ch;
            else if (ch == 's' || ch == 'S')
                return ch;
            else if (ch == 13)
                return ch;
        }
    }
}

int chooseMenu(int active)
{
    while (1)
    {
        drawMenu(active);
        char ch = readKeyMenu();
        switch (ch)
        {
        case 'w':
        case 'W':
            active--;
            if (active == 0) {active = 5;}
            break;
        case 's':
        case 'S':
            active++;
            if (active == 6) {active = 1;}
            break;
        case 13:
            return active;
        }
    }
}

void MENU(int active)
{
    system("cls");
    int item = chooseMenu(active);
    switch (item)
    {
    case 1:
        PLAY();
        break;
    case 2:
        MODE(1);
        break;
    case 3:
        SPEED();
        break;
    case 4:
        HIGHSCORE();
        break;
    case 5:
        ABOUT();
        break;
    }
}

//items menu

//PLAY()

//snake functions

void drawSnake(SNAKE snake)
{
    int i;
    textcolor(7);
    gotoxy(snake.tail[0].x, snake.tail[0].y);
    printf("%c", -24);
    textcolor(2);
    for (i = 1; i < snake.len - 1; i++)
    {
        gotoxy(snake.tail[i].x, snake.tail[i].y);
        printf("%c", 219);
    }
}

void clearSnake(SNAKE snake)
{
    gotoxy(snake.tail[snake.len - 1].x, snake.tail[snake.len - 1].y);
    textcolor(7);
    printf(" ");
}

void movingProcess(SNAKE *snake)
{
    int i;
    textcolor(7);
    for (i = snake->len - 1; i > 0; i--)
    {
        snake->tail[i].x = snake->tail[i - 1].x;
        snake->tail[i].y = snake->tail[i - 1].y;
    }
}

int getGold(SNAKE snake, COORDINATES gold)
{
    if (snake.tail[0].x == gold.x && snake.tail[0].y == gold.y)
        return 1;
    else
        return 0;
}

int isDuplicate(SNAKE snake)
{
    int i, j;
    for (i = 0; i < snake.len - 1; i++)
    {
        for (j = i + 1; j < snake.len; j++)
        {
            if ((snake.tail[i].x == snake.tail[j].x) && (snake.tail[i].y == snake.tail[j].y))
            {
                return 1;
            }
        }
    }
    return 0;
}

int isSameGoldAndSnake(SNAKE snake, COORDINATES gold)
{
    int i;
    for (i = 1; i < snake.len; i++)
    {
        if (gold.x == snake.tail[i].x && gold.y == snake.tail[i].y)
        {
            return 1;
        }
    }
    return 0;
}

void drawGold(COORDINATES gold)
{
    gotoxy(gold.x, gold.y);
    printf("X");
}

//end snake functions

//database

void whoop(int point)
{
    textcolor(12);
    gotoxy(53, 13);
    printf("Your score is %d", point);
    FILE *checkmode;
    FILE *checkscore;
    checkmode = fopen("Database/mode.txt", "r");
    int mode, high[5] = {0}, i, checknew = 0;
    fscanf(checkmode, "%d", &mode);
    fclose(checkmode);
    switch (mode)
    {
    case 1:
        checkscore = fopen("Database/h1.txt", "r");
        break;
    case 2:
        checkscore = fopen("Database/h2.txt", "r");
        break;
    case 3:
        checkscore = fopen("Database/h3.txt", "r");
        break;
    }
    for (i = 0; i < 5; i++)
    {
        fscanf(checkscore, "%d", &high[i]);
        if (point > high[i])
        {
            high[i] = point + high[i];
            point = high[i] - point;
            high[i] = high[i] - point;
            if (i == 0)
            {
                checknew = 2;
            }
            if (checknew != 2)
            {
                checknew = 1;
            }
        }
    }
    fclose(checkscore);
    switch (mode)
    {
    case 1:
        checkscore = fopen("Database/h1.txt", "w");
        break;
    case 2:
        checkscore = fopen("Database/h2.txt", "w");
        break;
    case 3:
        checkscore = fopen("Database/h3.txt", "w");
        break;
    }
    if (checknew )
    {
        gotoxy(53, 14);
        if (checknew == 2)
        {
        printf("The best record\n");
        }
        else
        {
            printf("New record\n");
        }
    }
    for (i = 0; i < 5; i++)
    {
        fprintf(checkscore, "%d\n", high[i]);
    }
    fclose(checkscore);
    gotoxy(47, 15);
    Sleep(1000);
    system("pause");
    MENU(1);
}

//end database

//campaign mode

//map 1

void drawMap1()
{
    int i;
    textcolor(3);
    for (i = 40; i < 81; i++)
    {
        gotoxy(i, 5);
        printf("%c", 219);
        gotoxy(i, 20);
        printf("%c", 219);
    }
    for (i = 6; i < 20; i++)
    {
        gotoxy(40, i);
        printf("%c", 219);
        gotoxy(80, i);
        printf("%c", 219);
    }
}

int isBoong(SNAKE snake)
{
    if (isDuplicate(snake))
        return 1;
    else if (snake.tail[0].x == 40 || snake.tail[0].x == 80)
        return 1;
    else if (snake.tail[0].y == 5 || snake.tail[0].y == 20)
        return 1;
    else
        return 0;
}

void MAP1()
{
    SNAKE snake;
    COORDINATES gold;
    STATUS statusCur;
    gold.x = (rand() % (80 - 41) + 41);
    gold.y = (rand() % (20 - 6) + 6);
    snake.tail[0].x = 45;
    snake.tail[0].y = 6;
    snake.tail[1].x = 44;
    snake.tail[1].y = 6;
    snake.tail[2].x = 43;
    snake.tail[2].y = 6;
    snake.len = 3;
    snake.status = RIGHT;
    int point = 0, bonus, time = 100, nextMap = 0;
    FILE *fspeed;
    fspeed = fopen("Database/speed.txt", "r");
    fscanf(fspeed, "%d", &time);
    fclose(fspeed);
    switch (time)
    {
        case 1:
            time = 70;
            bonus = 10;
            break;
        case 2:
            time = 40;
            bonus = 20;
            break;
        case 3:
            time = 10;
            bonus = 30;
            break;
    }
    drawMap1();
    textcolor(15);
    gotoxy(13, 6);
    printf("Press \"P\" to pause");
    while (1)
    {
        textcolor(15);
        gotoxy(13, 5);
        printf("Your score:  %d", point);
        drawSnake(snake);
        clearSnake(snake);
        if (nextMap == 14)
        {
            textcolor(14);
            drawGold(gold);
            textcolor(15);
        }
        else
        {
            drawGold(gold);
        }
        if (getGold(snake, gold))
        {
            nextMap++;
            snake.len++;
            point += bonus;
            do{
                gold.x = (rand() % (80 - 41) + 41);
                gold.y = (rand() % (20 - 6) + 6);
            } while (isSameGoldAndSnake(snake, gold));
        }
        if(kbhit())
        {
            char c = getch();
            switch (c)
            {
            case 27:
                MENU(1);
                break;
            case 'a':
            case 'A':
                if (snake.status != RIGHT) {snake.status = LEFT;}
                break;
            case 'd':
            case 'D':
                if (snake.status != LEFT) {snake.status = RIGHT;}
                break;
            case 'w':
            case 'W':
                if (snake.status != DOWN) {snake.status = UP;}
                break;
            case 's':
            case 'S':
                if (snake.status != UP) {snake.status = DOWN;}
                break;
            case 'p':
            case 'P':
                statusCur = snake.status;
                snake.status = STOP;
                break;
            }
        }
        if (snake.status != STOP)
        {
            movingProcess(&snake);
        }
        switch (snake.status)
        {
            case UP: snake.tail[0].y--; break;
            case DOWN: snake.tail[0].y++; break;
            case LEFT: snake.tail[0].x--; break;
            case RIGHT: snake.tail[0].x++; break;
            case STOP:
                textcolor(15);
                gotoxy(13, 6);
                printf("Press \"C\" to continue");
                char con = '#';
                do {
                    if (kbhit())
                    {
                        con = getch();
                    }
                } while (con != 'C' && con != 'c');
                gotoxy(13, 6);
                printf("Press \"P\" to pause   ");
                snake.status = statusCur;
        }
        if (isBoong(snake) || nextMap == 15) {break;}
        Sleep(time);
    }
    if (nextMap == 15)
    {
        MAP2(point, time, bonus);
    }
    else
    {
        whoop(point);
    }
}

//end map 1

//map 2

void drawMap2()
{
    int i;
    textcolor(3);
    for (i = 60; i < 81; i++)
    {
        gotoxy(i, 5);
        printf("%c", 219);
    }
    gotoxy(60, 9);
    for (i = 0; i < 10; i++)
    {
        printf("%c", 219);
    }
    for (i = 5; i < 10; i++)
    {
        gotoxy(60, i);
        printf("%c", 219);
    }
    gotoxy(51, 15);
    for (i = 0; i < 10; i++)
    {
        printf("%c", 219);
    }
    for (i = 20; i > 15; i--)
    {
        gotoxy(60, i);
        printf("%c", 219);
    }
    for (i = 40; i < 61; i++)
    {
        gotoxy(i, 20);
        printf("%c", 219);
    }
    for (i = 5; i < 21; i++)
    {
        gotoxy(40, i);
        printf("%c", 219);
        gotoxy(80, i);
        printf("%c", 219);
    }
}

int isBoongMap2(COORDINATES cor)
{
    if (cor.x == 40 || cor.x == 80)
    {
        return 1;
    }
    else if (cor.y == 5 && (cor.x > 59 && cor.x < 80))
    {
        return 1;
    }
    else if (cor.x == 60 && (cor.y > 5 && cor.y < 10))
    {
        return 1;
    }
    else if (cor.y == 9 &&(cor.x > 59 && cor.x < 70))
    {
        return 1;
    }
    else if (cor.y == 20 && (cor.x > 40 && cor.x < 61))
    {
        return 1;
    }
    else if (cor.x == 60 && (cor.y < 20 && cor.y > 14))
    {
        return 1;
    }
    else if (cor.y == 15 && (cor.x > 50 && cor.x < 61))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isTeleMap2(SNAKE snake)
{
    if ((snake.tail[0].x > 40 && snake.tail[0].x < 60) && snake.tail[0].y == 5)
    {
        return 1;
    }
    else if ((snake.tail[0].x > 60 && snake.tail[0].x < 80) && snake.tail[0].y == 20)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}

void MAP2(int point, int time, int bonus)
{
    system("cls");
    SNAKE snake;
    COORDINATES gold;
    STATUS statusCur;
    do {
        gold.x = (rand() % (80 - 41) + 41);
        gold.y = (rand() % (20 - 6) + 6);
    } while (isBoongMap2(gold));
    snake.tail[0].x = 45;
    snake.tail[0].y = 6;
    snake.tail[1].x = 44;
    snake.tail[1].y = 6;
    snake.tail[2].x = 43;
    snake.tail[2].y = 6;
    snake.len = 3;
    snake.status = RIGHT;
    int nextMap = 0;
    drawMap2();
    textcolor(15);
    gotoxy(13, 6);
    printf("Press \"P\" to pause");
    while (1)
    {
        textcolor(15);
        gotoxy(13, 5);
        printf("Your score:  %d", point);
        drawSnake(snake);
        clearSnake(snake);
        if (nextMap == 14)
        {
            textcolor(14);
            drawGold(gold);
            textcolor(15);
        }
        else
        {
            drawGold(gold);
        }
        if (getGold(snake, gold))
        {
            nextMap++;
            snake.len++;
            point += bonus;
            do{
                gold.x = (rand() % (80 - 41) + 41);
                gold.y = (rand() % (20 - 6) + 6);
            } while (isSameGoldAndSnake(snake, gold) || isBoongMap2(gold));
        }
        if(kbhit())
        {
            char c = getch();
            switch (c)
            {
            case 27:
                MENU(1);
                break;
            case 'a':
            case 'A':
                if (snake.status != RIGHT) {snake.status = LEFT;}
                break;
            case 'd':
            case 'D':
                if (snake.status != LEFT) {snake.status = RIGHT;}
                break;
            case 'w':
            case 'W':
                if (snake.status != DOWN) {snake.status = UP;}
                break;
            case 's':
            case 'S':
                if (snake.status != UP) {snake.status = DOWN;}
                break;
            case 'p':
            case 'P':
                statusCur = snake.status;
                snake.status = STOP;
                break;
            }
        }
        if (snake.status != STOP)
        {
            movingProcess(&snake);
        }
        switch (snake.status)
        {
            case UP: snake.tail[0].y--; break;
            case DOWN: snake.tail[0].y++; break;
            case LEFT: snake.tail[0].x--; break;
            case RIGHT: snake.tail[0].x++; break;
            case STOP:
                textcolor(15);
                gotoxy(13, 6);
                printf("Press \"C\" to continue");
                char con = '#';
                do {
                    if (kbhit())
                    {
                        con = getch();
                    }
                } while (con != 'C' && con != 'c');
                gotoxy(13, 6);
                printf("Press \"P\" to pause   ");
                snake.status = statusCur;
        }
        if (isBoongMap2(snake.tail[0]) || nextMap == 15) {break;}
        if (isTeleMap2(snake))
        {
            if (isTeleMap2(snake) == 1)
            {
                snake.tail[0].x += 20;
                snake.tail[0].y = 20;
            }
            else
            {
                snake.tail[0].x -= 20;
                snake.tail[0].y = 5;
            }
        }
        Sleep(time);
    }
    if (nextMap == 15)
    {
        MAP3(point, time, bonus);
    }
    else
    {
        whoop(point);
    }
}

//end map 2

//map 3

void drawMap3()
{
    int i;
    textcolor(3);
    gotoxy(40, 5);
    printf("%c", 219);
    gotoxy(80, 5);
    printf("%c", 219);
    for (i = 11; i < 16; i++)
    {
        if (i == 13)
        {
            continue;
        }
        gotoxy(40, i);
        printf("%c", 219);
        gotoxy(80, i);
        printf("%c", 219);
    }
    for (i = 41; i < 60; i++)
    {
        gotoxy(i, 12);
        printf("%c", 219);
    }
    for (i = 50; i < 71; i++)
    {
        gotoxy(i, 5);
        printf("%c", 219);
    }
    gotoxy(70, 12);
    for (i = 0; i < 10; i++)
    {
        printf("%c", 219);
    }

    for (i = 5; i < 13; i++)
    {
        gotoxy(60, i);
        printf("%c", 219);
    }
    gotoxy(41, 14);
    for (i = 0; i < 39; i++)
    {
        printf("%c", 219);
    }
    for (i = 40; i < 51; i++)
    {
        gotoxy(i, 20);
        printf("%c", 219);
    }
    for (i = 60; i < 71; i++)
    {
        gotoxy(i, 20);
        printf("%c", 219);
    }
    gotoxy(80, 20);
    printf("%c", 219);
    for (i = 15; i < 20; i++)
    {
        gotoxy(62, i);
        printf("%c", 219);
    }
}

int isBoongMap3(COORDINATES cor)
{
    if (cor.x == 40 && cor.y == 5)
    {
        return 1;
    }
    else if (cor.y == 5 && (cor.x > 49 && cor.x < 71))
    {
        return 1;
    }
    else if (cor.y == 5 && cor.x == 80)
    {
        return 1;
    }
    else if (cor.y == 11 && (cor.x == 40 || cor.x == 80))
    {
        return 1;
    }
    else if (cor.x == 60 && (cor.y > 5 && cor .y < 13))
    {
        return 1;
    }
    else if (cor.y == 12 && (cor.x > 39 && cor.x < 60))
    {
        return 1;
    }
    else if (cor.y == 12 && (cor.x > 69 && cor.x < 81))
    {
        return 1;
    }
    else if (cor.y == 14 && (cor.x > 39 && cor.x < 81))
    {
        return 1;
    }
    else if (cor.y == 15 && (cor.x == 40 || cor.x == 80))
    {
        return 1;
    }
    else if (cor.y == 20 && (cor.x > 39 && cor.x < 51))
    {
        return 1;
    }
    else if (cor.y == 20 && (cor.x > 59 && cor.x < 71))
    {
        return 1;
    }
    else if (cor.y == 20 && cor.x == 80)
    {
        return 1;
    }
    else if (cor.x == 62 && (cor.y > 14 && cor.y < 20))
    {
        return 1;
    }
    return 0;
}

int isTeleMap3(SNAKE snake)
{
    if (snake.tail[0].y == 5 && (snake.tail[0].x > 40 && snake.tail[0].x < 50))
    {
        return 1;
    }
    else if (snake.tail[0].y == 20 && (snake.tail[0].x > 50 && snake.tail[0].x < 60))
    {
        return 2;
    }
    else if (snake.tail[0].y == 5 && (snake.tail[0].x > 70 && snake.tail[0].x < 80))
    {
        return 3;
    }
    else if (snake.tail[0].y == 20 && (snake.tail[0].x > 70 && snake.tail[0].x < 80))
    {
        return 4;
    }
    else if (snake.tail[0].x == 40 && (snake.tail[0].y > 5 && snake.tail[0].y < 11))
    {
        return 5;
    }
    else if (snake.tail[0].x == 80 && (snake.tail[0].y > 5 && snake.tail[0].y < 11))
    {
        return 6;
    }
    else if (snake.tail[0].x == 40 && (snake.tail[0].y > 15 && snake.tail[0].y < 20))
    {
        return 7;
    }
    else if (snake.tail[0].x == 80 && (snake.tail[0].y > 15 && snake.tail[0].y < 20))
    {
        return 8;
    }
    else if (snake.tail[0].x == 40 && snake.tail[0].y == 13)
    {
        return 9;
    }
    else if (snake.tail[0].x == 80 && snake.tail[0].y == 13)
    {
        return 10;
    }
    else
    {
        return 0;
    }
}

void MAP3(int point, int time, int bonus)
{
    system("cls");
    SNAKE snake;
    COORDINATES gold;
    STATUS statusCur;
    do {
        gold.x = (rand() % (80 - 41) + 41);
        gold.y = (rand() % (20 - 6) + 6);
    } while (isBoongMap3(gold));
    snake.tail[0].x = 45;
    snake.tail[0].y = 13;
    snake.tail[1].x = 44;
    snake.tail[1].y = 13;
    snake.tail[2].x = 43;
    snake.tail[2].y = 13;
    snake.len = 3;
    snake.status = RIGHT;
    int end = 0;
    drawMap3();
    textcolor(15);
    gotoxy(13, 6);
    printf("Press \"P\" to pause");
    while (1)
    {
        textcolor(15);
        gotoxy(13, 5);
        printf("Your score:  %d", point);
        drawSnake(snake);
        clearSnake(snake);
        if (end == 14)
        {
            textcolor(14);
            drawGold(gold);
            textcolor(15);
        }
        else
        {
            drawGold(gold);
        }
        if (getGold(snake, gold))
        {
            end++;
            snake.len++;
            point += bonus;
            do{
                gold.x = (rand() % (80 - 41) + 41);
                gold.y = (rand() % (20 - 6) + 6);
            } while (isSameGoldAndSnake(snake, gold) || isBoongMap3(gold));
        }
        if(kbhit())
        {
            char c = getch();
            switch (c)
            {
            case 27:
                MENU(1);
                break;
            case 'a':
            case 'A':
                if (snake.status != RIGHT) {snake.status = LEFT;}
                break;
            case 'd':
            case 'D':
                if (snake.status != LEFT) {snake.status = RIGHT;}
                break;
            case 'w':
            case 'W':
                if (snake.status != DOWN) {snake.status = UP;}
                break;
            case 's':
            case 'S':
                if (snake.status != UP) {snake.status = DOWN;}
                break;
            case 'p':
            case 'P':
                statusCur = snake.status;
                snake.status = STOP;
                break;
            }
        }
        if (snake.status != STOP)
        {
            movingProcess(&snake);
        }
        switch (snake.status)
        {
            case UP: snake.tail[0].y--; break;
            case DOWN: snake.tail[0].y++; break;
            case LEFT: snake.tail[0].x--; break;
            case RIGHT: snake.tail[0].x++; break;
            case STOP:
                textcolor(15);
                gotoxy(13, 6);
                printf("Press \"C\" to continue");
                char con = '#';
                do {
                    if (kbhit())
                    {
                        con = getch();
                    }
                } while (con != 'C' && con != 'c');
                gotoxy(13, 6);
                printf("Press \"P\" to pause   ");
                snake.status = statusCur;
        }
        if (isBoongMap3(snake.tail[0]) || end == 15) {break;}
        if (isTeleMap3(snake))
        {
            switch (isTeleMap3(snake))
            {
            case 1:
                snake.tail[0].y = 20;
                snake.tail[0].x += 10;
                break;
            case 2:
                snake.tail[0].y = 5;
                snake.tail[0].x -= 10;
                break;
            case 3:
                snake.tail[0].y = 20;
                break;
            case 4:
                snake.tail[0].y = 5;
                break;
            case 5:
            case 7:
            case 9:
                snake.tail[0].x = 80;
                break;
            case 6:
            case 8:
            case 10:
                snake.tail[0].x = 40;
                break;
            }
        }
        Sleep(time);
    }
    if (end == 15)
    {
        victory(point);
    }
    else
    {
        whoop(point);
    }
}

//end map 3

//victory campaign
void victory(int point)
{
    system("cls");
    textcolor(12);
    gotoxy(53, 12);
    printf("YOU HAVE FINISHED CAMPAIGN");
    gotoxy(53, 13);
    printf("Your score is %d", point);
    FILE *checkscore;
    int high[5] = {0}, i, checknew = 0;
    checkscore = fopen("Database/h1.txt", "r");
    for (i = 0; i < 5; i++)
    {
        fscanf(checkscore, "%d", &high[i]);
        if (point > high[i])
        {
            high[i] = point + high[i];
            point = high[i] - point;
            high[i] = high[i] - point;
            if (i == 0)
            {
                checknew = 2;
            }
            if (checknew != 2)
            {
                checknew = 1;
            }
        }
    }
    fclose(checkscore);
    checkscore = fopen("Database/h1.txt", "w");
    if (checknew )
    {
        gotoxy(53, 14);
        if (checknew == 2)
        {
        printf("The best record\n");
        }
        else
        {
            printf("New record\n");
        }
    }
    for (i = 0; i < 5; i++)
    {
        fprintf(checkscore, "%d\n", high[i]);
    }
    fclose(checkscore);
    gotoxy(47, 15);
    Sleep(2000);
    getch();
    MENU(1);
}

void CAMPAIGN()
{
    MAP1();
}

//end campaign mode

//classic mode

void drawMapClassic()
{
    int i;
    textcolor(3);
    for (i = 40; i < 81; i++)
    {
        gotoxy(i, 5);
        printf("%c", 219);
        gotoxy(i, 20);
        printf("%c", 219);
    }
    for (i = 6; i < 20; i++)
    {
        gotoxy(40, i);
        printf("%c", 219);
        gotoxy(80, i);
        printf("%c", 219);
    }
}

int isTele(SNAKE snake)
{
    if (snake.tail[0].x == 40 || snake.tail[0].x == 80)
        return 1;
    else if (snake.tail[0].y == 5 || snake.tail[0].y == 20)
        return 2;
    else
        return 0;
}

void CLASSIC()
{
    COORDINATES gold;
    SNAKE snake;
    STATUS statusCur;
    gold.x = (rand() % (80 - 41) + 41);
    gold.y = (rand() % (20 - 6) + 6);
    snake.len = 3;
    snake.tail[0].x = 43;
    snake.tail[0].y = 6;
    snake.tail[1].x = 42;
    snake.tail[1].y = 6;
    snake.tail[2].x = 41;
    snake.tail[2].y = 6;
    snake.status = RIGHT;
    int point = 0, bonus, time = 100;
    FILE *fspeed;
    fspeed = fopen("Database/speed.txt", "r");
    fscanf(fspeed, "%d", &time);
    fclose(fspeed);
    switch (time)
    {
        case 1:
            time = 70;
            bonus = 10;
            break;
        case 2:
            time = 40;
            bonus = 20;
            break;
        case 3:
            time = 10;
            bonus = 30;
            break;
    }
    drawMapClassic();
    textcolor(15);
    gotoxy(13, 6);
    printf("Press \"P\" to pause");
    while (1)
    {
        textcolor(15);
        gotoxy(13, 5);
        printf("Your score:  %d", point);
        drawSnake(snake);
        clearSnake(snake);
        drawGold(gold);
        if (getGold(snake, gold))
        {
            snake.len++;
            point += bonus;
            do{
                gold.x = (rand() % (80 - 41) + 41);
                gold.y = (rand() % (20 - 6) + 6);
            } while (isSameGoldAndSnake(snake, gold));
        }
        if(kbhit())
        {
            char c = getch();
            switch (c)
            {
            case 27:
                MENU(1);
                break;
            case 'a':
            case 'A':
                if (snake.status != RIGHT) {snake.status = LEFT;}
                break;
            case 'd':
            case 'D':
                if (snake.status != LEFT) {snake.status = RIGHT;}
                break;
            case 'w':
            case 'W':
                if (snake.status != DOWN) {snake.status = UP;}
                break;
            case 's':
            case 'S':
                if (snake.status != UP) {snake.status = DOWN;}
                break;
            case 'p':
            case 'P':
                statusCur = snake.status;
                snake.status = STOP;
                break;
            }
        }
        if (snake.status != STOP)
        {
            movingProcess(&snake);
        }
        switch (snake.status)
        {
            case UP: snake.tail[0].y--; break;
            case DOWN: snake.tail[0].y++; break;
            case LEFT: snake.tail[0].x--; break;
            case RIGHT: snake.tail[0].x++; break;
            case STOP:
                textcolor(15);
                gotoxy(13, 6);
                printf("Press \"C\" to continue");
                char con = '#';
                do {
                    if (kbhit())
                    {
                        con = getch();
                    }
                } while (con != 'C' && con != 'c');
                gotoxy(13, 6);
                printf("Press \"P\" to pause   ");
                snake.status = statusCur;
        }
        Sleep(time);
        if (isDuplicate(snake)) {break;}
        if (isTele(snake))
        {
           if (isTele(snake) == 1)
           {
                if (snake.tail[0].x == 40)
                {
                    snake.tail[0].x = 79;
                }
                else
                {
                    snake.tail[0].x = 41;
                }
           }
           else
           {
               if (snake.tail[0].y == 5)
               {
                   snake.tail[0].y = 19;
               }
               else
               {
                   snake.tail[0].y = 6;
               }
           }
        }
    }
    whoop(point);
}

//end classic mode

//slither mode

void drawSnakeAI(SNAKE snake)
{
    int i;
    textcolor(7);
    gotoxy(snake.tail[0].x, snake.tail[0].y);
    printf("%c", -24);
    textcolor(13);
    for (i = 1; i < snake.len - 1; i++)
    {
        gotoxy(snake.tail[i].x, snake.tail[i].y);
        printf("%c", 219);
    }
}

int isBoongSlither(SNAKE snake)
{
    if (snake.tail[0].x == 40 || snake.tail[0].x == 80)
    {
        return 1;
    }
    else if (snake.tail[0].y == 20 || snake.tail[0].y == 5)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isBeKilled(SNAKE snake, COORDINATES head)
{
    int i;
    for (i = 0; i < snake.len; i++)
    {
        if (snake.tail[i].x == head.x && snake.tail[i].y == head.y)
        {
            return 1;
        }
    }
    return 0;
}

void SLITHER()
{
    COORDINATES gold;
    SNAKE snake;
    SNAKE comSnake[3];
    STATUS statusCur;
    gold.x = (rand() % (80 - 41) + 41);
    gold.y = (rand() % (20 - 6) + 6);
    snake.len = 3;
    snake.tail[0].x = 43;
    snake.tail[0].y = 6;
    snake.tail[1].x = 42;
    snake.tail[1].y = 6;
    snake.tail[2].x = 41;
    snake.tail[2].y = 6;
    snake.status = RIGHT;
    int point = 0, bonus, time = 100, com, changeStatusAI = 0;

    for (com = 0; com < 3; com++)
    {
        comSnake[com].len = 7;
        comSnake[com].tail[0].x = 70;
        comSnake[com].tail[0].y = 18 - com;
        comSnake[com].tail[1].x = 71;
        comSnake[com].tail[1].y = 18 - com;
        comSnake[com].tail[2].x = 72;
        comSnake[com].tail[2].y = 18 - com;
        comSnake[com].tail[3].x = 73;
        comSnake[com].tail[3].y = 18 - com;
        comSnake[com].tail[4].x = 74;
        comSnake[com].tail[4].y = 18 - com;
        comSnake[com].tail[5].x = 75;
        comSnake[com].tail[5].y = 18 - com;
        comSnake[com].tail[6].x = 76;
        comSnake[com].tail[6].y = 18 - com;
        comSnake[com].status = LEFT;
    }

    FILE *fspeed;
    fspeed = fopen("Database/speed.txt", "r");
    fscanf(fspeed, "%d", &time);
    fclose(fspeed);
    switch (time)
    {
        case 1:
            time = 70;
            bonus = 10;
            break;
        case 2:
            time = 40;
            bonus = 20;
            break;
        case 3:
            time = 10;
            bonus = 30;
            break;
    }
    drawMapClassic();
    textcolor(15);
    gotoxy(13, 6);
    printf("Press \"P\" to pause");
    while (1)
    {
        textcolor(15);
        gotoxy(13, 5);
        printf("Your score:  %d", point);
        drawSnake(snake);
        clearSnake(snake);
        for (com = 0; com < 3; com++)
        {
            drawSnakeAI(comSnake[com]);
            clearSnake(comSnake[com]);
        }
        drawGold(gold);
        if (getGold(snake, gold))
        {
            snake.len++;
            point += bonus;
            do{
                gold.x = (rand() % (80 - 41) + 41);
                gold.y = (rand() % (20 - 6) + 6);
            } while (isSameGoldAndSnake(snake, gold));
            drawGold(gold);
        }
        for (com = 0; com < 3; com++)
        {
            if (getGold(comSnake[com], gold))
            {
                comSnake[com].len++;
                do{
                    gold.x = (rand() % (80 - 41) + 41);
                    gold.y = (rand() % (20 - 6) + 6);
                } while (isSameGoldAndSnake(comSnake[com], gold));
            }
            if (isBeKilled(snake, comSnake[com].tail[0]))
            {
                snake.len += 2;
                point += bonus * 2;
                int clear;
                textcolor(7);
                for (clear = 0; clear < comSnake[com].len; clear++)
                {
                    gotoxy(comSnake[com].tail[clear].x, comSnake[com].tail[clear].y);
                    printf(" ");
                }
                comSnake[com].tail[0].x = 70;
                comSnake[com].tail[0].y = 18 - com;
                comSnake[com].tail[1].x = 71;
                comSnake[com].tail[1].y = 18 - com;
                comSnake[com].tail[2].x = 72;
                comSnake[com].tail[2].y = 18 - com;
                comSnake[com].tail[3].x = 73;
                comSnake[com].tail[3].y = 18 - com;
                comSnake[com].tail[4].x = 74;
                comSnake[com].tail[4].y = 18 - com;
                comSnake[com].tail[5].x = 75;
                comSnake[com].tail[5].y = 18 - com;
                comSnake[com].tail[6].x = 76;
                comSnake[com].tail[6].y = 18 - com;
            }
        }
        if(kbhit())
        {
            char c = getch();
            switch (c)
            {
            case 27:
                MENU(1);
                break;
            case 'a':
            case 'A':
                if (snake.status != RIGHT) {snake.status = LEFT;}
                break;
            case 'd':
            case 'D':
                if (snake.status != LEFT) {snake.status = RIGHT;}
                break;
            case 'w':
            case 'W':
                if (snake.status != DOWN) {snake.status = UP;}
                break;
            case 's':
            case 'S':
                if (snake.status != UP) {snake.status = DOWN;}
                break;
            case 'p':
            case 'P':
                statusCur = snake.status;
                snake.status = STOP;
                break;
            }
        }
        if (changeStatusAI == 5)
        {
            for (com = 0; com < 3; com++)
            {
                switch((rand() % 4 + 1))
                {
                case 1:
                    if (comSnake[com].status != RIGHT) {comSnake[com].status = LEFT;}
                    break;
                case 2:
                    if (comSnake[com].status != LEFT) {comSnake[com].status = RIGHT;}
                    break;
                case 3:
                    if (comSnake[com].status != DOWN) {comSnake[com].status = UP;}
                    break;
                case 4:
                    if (comSnake[com].status != UP) {comSnake[com].status = DOWN;}
                    break;
                }
            }
            changeStatusAI = 0;
        }
        if (snake.status != STOP)
        {
            movingProcess(&snake);
        }
        switch (snake.status)
        {
            case UP: snake.tail[0].y--; break;
            case DOWN: snake.tail[0].y++; break;
            case LEFT: snake.tail[0].x--; break;
            case RIGHT: snake.tail[0].x++; break;
            case STOP:
                textcolor(15);
                gotoxy(13, 6);
                printf("Press \"C\" to continue");
                char con = '#';
                do {
                    if (kbhit())
                    {
                        con = getch();
                    }
                } while (con != 'C' && con != 'c');
                gotoxy(13, 6);
                printf("Press \"P\" to pause   ");
                snake.status = statusCur;
        }
        for (com = 0; com < 3; com++)
        {
            if (comSnake[com].status != STOP)
            {
                movingProcess(&comSnake[com]);
            }
            switch (comSnake[com].status)
            {
            case UP: comSnake[com].tail[0].y--; break;
            case DOWN: comSnake[com].tail[0].y++; break;
            case LEFT: comSnake[com].tail[0].x--; break;
            case RIGHT: comSnake[com].tail[0].x++; break;
            case STOP:
                break;
            }
        }
        Sleep(time);
        for (com = 0; com < 3; com++)
        {
            if (isBeKilled(comSnake[com], snake.tail[0]))
            {
                whoop(point);
            }
        }
        if (isDuplicate(snake) || isBoongSlither(snake)) {break;}
        for (com = 0; com < 3; com++)
        {
            if (isTele(comSnake[com]))
            {
               if (isTele(comSnake[com]) == 1)
               {
                    if (comSnake[com].tail[0].x == 40)
                    {
                        comSnake[com].tail[0].x = 79;
                    }
                    else
                    {
                        comSnake[com].tail[0].x = 41;
                    }
               }
               else
               {
                   if (comSnake[com].tail[0].y == 5)
                   {
                       comSnake[com].tail[0].y = 19;
                   }
                   else
                   {
                       comSnake[com].tail[0].y = 6;
                   }
               }
            }
        }
        changeStatusAI++;
    }
    whoop(point);
}

//end slither mode

void PLAY()
{
    system("cls");
    int playMode = 1;
    FILE *fmode;
    fmode = fopen("Database/mode.txt", "r");
    fscanf(fmode, "%d", &playMode);
    fclose(fmode);
    switch (playMode)
    {
    case 1:
        CAMPAIGN();
        break;
    case 2:
        CLASSIC();
        break;
    case 3:
        SLITHER();
        break;
    }
}

//end PLAY()

//MODE()

char readKeyMode()
{
    while (1)
    {
        if (kbhit())
        {
            char ch = getch();
            if (ch == 'w' || ch == 'W')
                return ch;
            else if (ch == 's' || ch == 'S')
                return ch;
            else if (ch == 13)
                return ch;
            else if (ch == 27)
                return ch;
        }
    }
}

void drawMode(int active)
{
    //draw background
    drawBackground(38, 3, 49, 15, 34);

    int i, y = 9;

    //draw title
    textcolor(11);
    gotoxy(55, 4);
    printf("                ");
    gotoxy(55, 5);
    printf("      MODE      ");
    gotoxy(55, 6);
    printf("                ");

    //draw mode
    textcolor(15);
    for (i = 0; i < 3; i++)
    {
        if (i + 1 == active)
        {
            textcolor(12);
            gotoxy(48, y);
            switch (i)
            {
                case 0:
                    printf("       C A M P A I G N       ");
                    break;
                case 1:
                    printf("        C L A S S I C        ");
                    break;
                case 2:
                    printf("        S L I T H E R        ");
                    break;
            }
            y += 2;
            textcolor(15);
            continue;
        }
        gotoxy(48, y);
        switch (i)
        {
            case 0:
                printf("          CAMPAIGN           ");
                break;
            case 1:
                printf("          CLASSIC            ");
                break;
            case 2:
                printf("          SLITHER            ");
                break;
        }
        y += 2;
    }

    //draw copyright
    textcolor(14);
    gotoxy(40, 16);
    printf(" COPYRIGHT 2017 Gr^k-T. ALL RIGHTS RESERVED. ");
}

int chooseMode(int active)
{
    while (1)
    {
        drawMode(active);
        char ch = readKeyMode();
        switch (ch)
        {
        case 'w':
        case 'W':
            active--;
            if (active == 0) {active = 3;}
            break;
        case 's':
        case 'S':
            active++;
            if (active == 4) {active = 1;}
            break;
        case 13:
            return active;
        case 27:
            return 27;
        }
    }
}

void MODE(int active)
{
    system("cls");
    int mode = chooseMode(active), modeCur;
    FILE *fmode;
    fmode = fopen("Database/mode.txt", "r");
    fscanf(fmode, "%d", &modeCur);
    fclose(fmode);
    switch (mode)
    {
    case 1:
        if (modeCur != 1)
        {
            FILE *fmode;
            fmode = fopen("Database/mode.txt", "w");
            fprintf(fmode, "%d", 1);
            fclose(fmode);
        }
        MODE(1);
        break;
    case 2:
        if (modeCur != 2)
        {
            FILE *fmode;
            fmode = fopen("Database/mode.txt", "w");
            fprintf(fmode, "%d", 2);
            fclose(fmode);
        }
        MODE(2);
        break;
    case 3:
        if (modeCur != 3)
        {
            FILE *fmode;
            fmode = fopen("Database/mode.txt", "w");
            fprintf(fmode, "%d", 3);
            fclose(fmode);
        }
        MODE(3);
        break;
    case 4:
        if (modeCur != 4)
        {
            FILE *fmode;
            fmode = fopen("Database/mode.txt", "w");
            fprintf(fmode, "%d", 4);
            fclose(fmode);
        }
        MODE(4);
        break;
    default:
        MENU(2);
    }
}

//end MODE()


//SPEED()

char readKeySpeed()
{
    while (1)
    {
        if (kbhit())
        {
            char ch = getch();
            if (ch == 'a' || ch == 'A')
            {
                return 'a';
            }
            else if (ch == 'd' || ch == 'D')
            {
                return 'd';
            }
            else if (ch == 13)
            {
                return 13;
            }
            else if (ch == 27)
            {
                return 27;
            }
        }
    }
}

void drawLevel(int level)
{
    int i, y = 0;
    textcolor(66);
    for (i = 0; i < 3; i++)
    {
        if (i == level)
        {
            textcolor(34);
        }
        gotoxy(52 + y, 9);
        printf("        ");
        y += 7;
    }
    textcolor(15);
}

void SPEED()
{
    system("cls");
    int speedCur = 1;
    FILE *fdataspeed;
    fdataspeed = fopen("Database/speed.txt", "r");
    fscanf(fdataspeed, "%d", &speedCur);
    fclose(fdataspeed);
    drawBackground(38, 3, 49, 12, 34);
    textcolor(11);
    gotoxy(55, 4);
    printf("                ");
    gotoxy(55, 5);
    printf("     SPEED      ");
    gotoxy(55, 6);
    printf("                ");

    drawLevel(speedCur);

    textcolor(14);
    gotoxy(40, 13);
    printf(" COPYRIGHT 2017 Gr^k-T. ALL RIGHTS RESERVED. ");

    FILE *feditspeed;
    int speedSave = speedCur;
    while (1)
    {
        char ch = readKeySpeed();
        switch (ch)
        {
        case 'a':
            speedCur--;
            if (speedCur == 0)
            {
                speedCur = 1;
            }
            break;
        case 'd':
            speedCur++;
            if (speedCur == 4)
            {
                speedCur = 3;
            }
            break;
        case 13:
            speedSave = speedCur;
            feditspeed = fopen("Database/speed.txt", "w");
            fprintf(feditspeed, "%d", speedSave);
            fclose(feditspeed);
            break;
        case 27:
            feditspeed = fopen("Database/speed.txt", "w");
            fprintf(feditspeed, "%d", speedSave);
            fclose(feditspeed);
            MENU(3);
            break;
        }
        drawLevel(speedCur);
    }
}

//end SPEED()

//HIGHSCORE()

void drawHighScore(int mode)
{
    int high[5], i, y = 9;
    FILE *fdatahigh;
    switch (mode)
    {
    case 1:
        fdatahigh = fopen("Database/h1.txt", "r");
        for (i = 0; i < 5; i++)
        {
            fscanf(fdatahigh, "%d", &high[i]);
        }
        break;
    case 2:
        fdatahigh = fopen("Database/h2.txt", "r");
        for (i = 0; i < 5; i++)
        {
            fscanf(fdatahigh, "%d", &high[i]);
        }
        break;
    case 3:
        fdatahigh = fopen("Database/h3.txt", "r");
        for (i = 0; i < 5; i++)
        {
            fscanf(fdatahigh, "%d", &high[i]);
        }
        break;
    }
    fclose(fdatahigh);
    drawBackground(38, 3, 49, 19, 34);
    textcolor(11);
    gotoxy(55, 4);
    printf("                ");
    gotoxy(55, 5);
    printf("   HIGH SCORE   ");
    gotoxy(55, 6);
    printf("                ");

    //draw highscore
    textcolor(15);
    for (i = 0; i < 5; i++)
    {
        gotoxy(58, y);
        printf("   %d   ", high[i]);
        y += 2;
    }

    //draw copyright
    textcolor(14);
    gotoxy(40, 20);
    printf(" COPYRIGHT 2017 Gr^k-T. ALL RIGHTS RESERVED. ");
    while (1)
    {
        if (kbhit())
        {
            char ch = getch();
            if (ch == 27)
            {
                MENU(4);
            }
        }
    }
}

void HIGHSCORE()
{
    system("cls");
    int highCur;
    FILE *fmode;
    fmode = fopen("Database/mode.txt", "r");
    fscanf(fmode, "%d", &highCur);
    fclose(fmode);
    switch (highCur)
    {
    case 1:
        drawHighScore(1);
        break;
    case 2:
        drawHighScore(2);
        break;
    case 3:
        drawHighScore(3);
        break;
    case 4:
        drawHighScore(4);
        break;
    }
}

//end HIGHSCORE()

//ABOUT()
void ABOUT()
{
    system("cls");
    gotoxy(45, 15);
    printf("Waiting for a second... ");
    system("start https://www.facebook.com/356a4368");
    while (1)
        if (kbhit())
        {
            char c = getch();
            if (c == 27)
            {
                MENU(5);
            }
        }
}

//end ABOUT()

//end menu process

int main()
{
    SetConsoleTitle(" Snake Game Console");
    ShowCur(0);
    MENU(1);
    return 0;
}
