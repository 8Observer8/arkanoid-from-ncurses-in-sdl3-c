#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct { float x; float y; } Xyz;
typedef struct { Xyz pos; Xyz dir; int w; int h; } Ball;
typedef struct { Xyz pos; int w; int h; } Racket;

#define new_xyz(x, y)    ((Xyz) {x, y})
#define add_xyz(a, b)    new_xyz(a.x+b.x, a.y+b.y)

void init_ball(void);
void move_ball(void);
void init_racket(void);
void move_racket(int);
void draw_field(void);
void draw_racket(void);
void draw_ball(void);

Ball ball;                             // мячик
Racket racket;                         // ракетка
int shift = 3;                         // скорость ракетки
int WIDTH = 400, HEIGHT = 300;         // ширина и высота холста
int border_w = 20;                     // ширина границы
bool left = false;                     // направление - влево
bool right = false;                    // направление - вправо
bool game_over = false;                // конец игры

static SDL_Window *window = NULL;      // окно
static SDL_Renderer *renderer = NULL;  // рисовальщик

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Arkanoid", WIDTH, HEIGHT, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    init_ball();                                    // положение мяча
    init_racket();                                  // положение ракетки

    SDL_SetRenderVSync(renderer, 1);
    return SDL_APP_CONTINUE;
}
// ------------------------------------------------------------
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT)
        return SDL_APP_SUCCESS;
    else if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.scancode == SDL_SCANCODE_LEFT)
            left = true;
        else if(event->key.scancode == SDL_SCANCODE_RIGHT)
            right = true;
    } else if (event->type == SDL_EVENT_KEY_UP) {
        if (event->key.scancode == SDL_SCANCODE_LEFT)
            left = false;
        else if(event->key.scancode == SDL_SCANCODE_RIGHT)
            right = false;
    }
    return SDL_APP_CONTINUE;
}
// ------------------------------------------------------------
SDL_AppResult SDL_AppIterate(void *appstate) {
    // Управление ракеткой
    if (left && !game_over) {
        move_racket(-shift);
    } else if (right && !game_over)
        move_racket(shift);

    SDL_SetRenderDrawColor(renderer, 33, 33, 33, SDL_ALPHA_OPAQUE); // цвет холста
    SDL_RenderClear(renderer); // очищаем холст и заливаем его цветом холста

    if (!game_over)
        move_ball();
    draw_field();  // рисуем рамку
    draw_racket(); // рисуем ракетку
    draw_ball();   // рисуем мяч

    SDL_RenderPresent(renderer); // выводим содержимое рисовальщика на экран
    return SDL_APP_CONTINUE; // продолжить выполнять программу!
}
// ------------------------------------------------------------
void SDL_AppQuit(void *appstate, SDL_AppResult result) {}
// ------------------------------------------------------------
void init_ball(void) {
    Xyz speed = new_xyz(1, 2);                                  // вектор движения
    Xyz ball_position = new_xyz(WIDTH >> 1, HEIGHT - 100);      // позиция мяча
    Xyz ball_direction = new_xyz(-speed.x, -speed.y);
    ball = (Ball) { ball_position, ball_direction, 20, 20 };
}
// ------------------------------------------------------------
void move_ball(void) {
    Xyz b_pos = add_xyz(ball.pos, ball.dir);                    // позиция шарика
    Xyz r_pos = new_xyz(racket.pos.x, racket.pos.x + racket.w); // позиция ракетки

    if (b_pos.x <= border_w || b_pos.x + ball.w >= WIDTH - border_w)
        ball.dir.x = -ball.dir.x;
    else if (b_pos.y <= border_w)
        ball.dir.y = -ball.dir.y;
    else if (b_pos.y + ball.w >= racket.pos.y && (racket.pos.x <= b_pos.x + ball.w && b_pos.x <= racket.pos.x + racket.w))
        ball.dir.y = -ball.dir.y;
    else if (b_pos.y + ball.w >= racket.pos.y) {
        SDL_Log("Game over");
        game_over = true;
    }

    ball.pos = b_pos;
}
// ------------------------------------------------------------
void init_racket(void) {
    Xyz pos = new_xyz((WIDTH >> 1) - 50, HEIGHT - 30);
    racket = (Racket) { pos, 100, 20 };
}
// ------------------------------------------------------------
void move_racket(int shift) {
    int x = (int) racket.pos.x + shift;
    if (x < border_w)
        x = border_w;
    else if (x > WIDTH - border_w - racket.w)
        x = WIDTH - border_w - racket.w;
    racket.pos.x = x;
}
// ------------------------------------------------------------
void draw_field(void) {
    // Левая граница
    SDL_FRect leftBorder = { 0, 0, 20, 300 };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);  // цвет рисования
    SDL_RenderFillRect(renderer, &leftBorder); // рисуем прямоугольник заданным цветом
    // Верхняя граница
    SDL_FRect topBorder = { 20, 0, 380, 20 };
    SDL_RenderFillRect(renderer, &topBorder); // рисуем прямоугольник заданным цветом
    // Правая граница
    SDL_FRect rightBorder = { 380, 0, 20, 300 };
    SDL_RenderFillRect(renderer, &rightBorder); // рисуем прямоугольник заданным цветом
}
// ------------------------------------------------------------
void draw_racket(void) {
    SDL_FRect rect = { racket.pos.x, racket.pos.y, racket.w, racket.h };
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);  // цвет рисования
    SDL_RenderFillRect(renderer, &rect); // рисуем прямоугольник заданным цветом
}
// ------------------------------------------------------------
void draw_ball(void) {
    SDL_FRect rect = { ball.pos.x, ball.pos.y, ball.w, ball.h };
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);  // цвет рисования
    SDL_RenderFillRect(renderer, &rect); // рисуем прямоугольник заданным цветом
}
// ------------------------------------------------------------
