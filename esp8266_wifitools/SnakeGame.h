#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "A_config.h"

#include "src/SimpleButton/SimpleButton.h"

#define SCREEN_WIDTH        128 // OLED display width, in pixels
#define SCREEN_HEIGHT        64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET            -1 // Reset pin # (or -1 if sharing Arduino reset pin)

#define SNAKE_PIECE_SIZE     3
#define MAX_SANKE_LENGTH   165
#define MAP_SIZE_X          20
#define MAP_SIZE_Y          20
#define STARTING_SNAKE_SIZE  5
#define SNAKE_MOVE_DELAY    30

typedef enum
{
    START,
    RUNNING,
    GAMEOVER
} State;

typedef enum
{
    LEFT,
    UP,
    RIGHT,
    DOWN
} Direction;
namespace SnakeGame
{
        // SnakeGame.h
    extern Adafruit_SSD1306 display;
    extern int8_t snake[MAX_SANKE_LENGTH][2];
    extern uint8_t snake_length;
    extern Direction dir;
    extern int newDir;
    extern int8_t fruit[2];
    extern bool snakeStart;
    extern State gameState;
    extern int moveTime;


    void StartSnake();
    void setupGame();

    void resetSnake();
    void SnakeUpdate();
    void KillGame();

    bool buttonPress();
    void readDirection();
    bool moveSnake();
    void checkFruit();
    void generateFruit();
    bool collisionCheck(int8_t x, int8_t y);
    void drawMap();
    void drawScore();
    void drawPressToStart();
    void drawGameover();
} // namespace SnakeGame
