#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<conio.h>
#define MAP "¡ö "
#define SNAKE "¡Ñ"
#define FOOD "¡ô"
#define HIGHT 15
#define LEN 52
#define SNAKE_LEN_MAX 300
struct food{
    int x[300];
    int y[300];
    int n;
}food;
struct snake{
    float speed;
    int len;
    int len_tmp;
    int x[SNAKE_LEN_MAX];
    int y[SNAKE_LEN_MAX];
    int state ;        //0:normal  1:lenthening  2:dead
}snake;
int starttime;
int gametime = 0;    //ms
int highest= 0;
void gotoxy(int x, int y) {
    COORD pos = { x,y };
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hOut, pos);
}
void create_map(){
    for (int k = 1; k <= HIGHT; k+=1){
        gotoxy(0, k);
        printf(MAP);
        gotoxy(LEN, k);
        printf(MAP);
    }
    for (int i = 0; i <= LEN; i +=2 ){
        gotoxy(i, 0);
        printf(MAP);
        gotoxy(i,HIGHT);
        printf(MAP);
    }
}
void create_snake(){
    gotoxy(26, 7);
    printf(SNAKE);
}
void create_food(int n){
    int j = 0;
    srand((unsigned)time(NULL));
    food.x[n] = 1 + rand() % (LEN - 1);
    food.y[n] = 1 + rand() % (HIGHT - 1);
    for (int i = 0; i <= snake.len_tmp; i++) {
        if (snake.x[i] == food.x[n] && snake.y[i] == food.y[n])
            j = 1;
    }
    if (food.x[n] % 2 == 0&&j==0) {
        gotoxy(food.x[n], food.y[n]);
        printf(FOOD);
        food.n+=1;
        gotoxy(60, 0);
    }
    //else
    //    create_food(n);
}
void delete(int x, int y){
    gotoxy(x, y);
    printf(" ");
}
int key;
void input_key(){
    if (_kbhit())
    {
        fflush(stdin);
        int t = _getch();     //w:119£¬s:115,d:100,a:97
        if (t == 119 || t == 115 || t == 100 | t == 97)
            key = t;
        gotoxy(60, 0);
    }
}
void move_snake(){
    if (snake.len_tmp == snake.len){
        delete(snake.x[snake.len], snake.y[snake.len]);
        snake.state = 0;
    }
    if (snake.len_tmp < snake.len && (key == 119 || key == 115 || key == 100 || key == 97)){
        snake.len_tmp++;
    }
    for (int i = snake.len_tmp; i >=1 ; i--){
        snake.x[i] = snake.x[i - 1];
        snake.y[i] = snake.y[i - 1];
    }
    switch (key) {
    case 119:
            snake.y[0]--;
            break;
    case 115:
            snake.y[0]++;
            break;
    case 100:
            snake.x[0] += 2;
            break;
    case 97:
            snake.x[0] -= 2;
            break;
    }
    for (int i= 0; i <= snake.len_tmp; i++){
        gotoxy(snake.x[i], snake.y[i]);
        printf(SNAKE);
    }
    gotoxy(60, 0);
}
void judge_state(){
    for (int n = 0; n <= food.n; n++) {
        if (snake.x[0] == food.x[n] && snake.y[0] == food.y[n]) {
            snake.len += 1;
            snake.state = 1;
            food.x[n] = 0;
            food.y[n] = 0;
            if(snake.speed>=200)
                snake.speed *= 0.96;
            else if(snake.speed>=100)
                snake.speed *= 0.988;
        }
    }
    if (snake.x[0] >= LEN || snake.x[0] <= 0 || snake.y[0] >= HIGHT || snake.y[0] <= 0) {
        snake.state = 2;
        gotoxy(snake.x[0], snake.y[0]);
        printf(MAP);
    }
    else
        snake.state = 0;
    for (int i = 1; i <= snake.len_tmp; i++) {
        if (snake.x[i] == snake.x[0] && snake.y[i] == snake.y[0])
            snake.state = 2;
    }
}
void initialization() {
    key = 0;
    snake.speed = 250;
    snake.len = 2;
    snake.len_tmp = 0;
    snake.x[0] = 26; snake.y[0] = 7;
    snake.state = 1;
    food.n = 2;
    starttime = (unsigned)time(NULL);
    fflush(stdin);
    create_map();
    create_snake();
    create_food(0);
    create_food(1);
    gotoxy(60, 0);
}
void print_data()
{
    gotoxy(60, 1);
    printf("gametime%d  ", gametime);
    gotoxy(60, 2);
    printf("len%d ", snake.len);
    gotoxy(60, 3);
    printf("speed%.2f", (250/snake.speed));
}
int main(){
    while (1) {
        initialization();
        while (1) {
            input_key();
            move_snake();
            judge_state();
            gametime = (int)time(NULL) - starttime;
            if (gametime % 2 == 0)
                create_food(food.n);
            if (snake.state == 2) {
                if (snake.len > highest)
                    highest = snake.len;
                gotoxy(16, 7);
                printf("GameOver!Snake lenth is %d",snake.len);
                gotoxy(16, 8);
                printf("The highest is%d ", highest);
                gotoxy(60, 0);
                break;
            }
            print_data();
            Sleep((int)snake.speed);
        }
        system("pause");
        system("cls");
    }
    return 0;
}