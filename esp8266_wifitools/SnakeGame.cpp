#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "A_config.h"

#include "SnakeGame.h"

// SnakeGame.cpp
Adafruit_SSD1306 SnakeGame::display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int8_t SnakeGame::snake[MAX_SANKE_LENGTH][2];
uint8_t SnakeGame::snake_length;
Direction SnakeGame::dir;
int SnakeGame::newDir;
int8_t SnakeGame::fruit[2];
bool SnakeGame::snakeStart;
State SnakeGame::gameState;
int SnakeGame::moveTime;

void SnakeGame::StartSnake()
{
  // Inisialisasi komunikasi I2C
  Wire.begin(I2C_SDA, I2C_SCL);

  // Inisialisasi display OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  snakeStart = true;
  setupGame();
}
void SnakeGame::setupGame()
{
  gameState = START;
  dir = DOWN;
  newDir = DOWN;
  resetSnake();
  generateFruit();
  display.clearDisplay();
  drawMap();
  drawScore();
  drawPressToStart();
  display.display();
}

void SnakeGame::KillGame()
{
  snakeStart = false;
  resetSnake();
}
void SnakeGame::resetSnake()
{
  snake_length = STARTING_SNAKE_SIZE;
  for (int i = 0; i < snake_length; i++)
  {
    snake[i][0] = MAP_SIZE_X / 2 - i;
    snake[i][1] = MAP_SIZE_Y / 2;
  }
}

void SnakeGame::SnakeUpdate()
{
  if (snakeStart)
  {
    switch (gameState)
    {
    case START:
      if (buttonPress())
        gameState = RUNNING;
      break;

    case RUNNING:
      moveTime++;
      readDirection();
      if (moveTime >= SNAKE_MOVE_DELAY)
      {
        dir = (Direction)newDir;
        display.clearDisplay();
        if (moveSnake())
        {
          gameState = GAMEOVER;
          drawGameover();
          delay(1000);
        }
        drawMap();
        drawScore();
        display.display();
        checkFruit();
        moveTime = 0;
      }
      break;

    case GAMEOVER:
      if (buttonPress())
      {
        delay(500);
        setupGame();
        gameState = START;
      }
      break;
    }

    delay(10);
  }
}

bool SnakeGame::buttonPress()
{
  if (digitalRead(BUTTON_A) == LOW)
  {
    return true;
  }
  return false;
}

void SnakeGame::readDirection()
{
  if (digitalRead(BUTTON_UP) == LOW)
  {
    newDir++;
    return;
  }
}

bool SnakeGame::moveSnake()
{
  int8_t x = snake[0][0];
  int8_t y = snake[0][1];
  if (!(dir > 4))
  {
    switch (dir)
    {
    case LEFT:
      x -= 1;
      break;
    case UP:
      y -= 1;
      break;
    case RIGHT:
      x += 1;
      break;
    case DOWN:
      y += 1;
      break;
    }
  }
  else
  {
    newDir = 0;
  }

  if (collisionCheck(x, y))
    return true;

  for (int i = snake_length - 1; i > 0; i--)
  {
    snake[i][0] = snake[i - 1][0];
    snake[i][1] = snake[i - 1][1];
  }

  snake[0][0] = x;
  snake[0][1] = y;
  return false;
}

void SnakeGame::checkFruit()
{
  if (fruit[0] == snake[0][0] && fruit[1] == snake[0][1])
  {
    if (snake_length + 1 <= MAX_SANKE_LENGTH)
      snake_length++;
    generateFruit();
  }
}

void SnakeGame::generateFruit()
{
  bool b = false;
  do
  {
    b = false;
    fruit[0] = random(0, MAP_SIZE_X);
    fruit[1] = random(0, MAP_SIZE_Y);
    for (int i = 0; i < snake_length; i++)
    {
      if (fruit[0] == snake[i][0] && fruit[1] == snake[i][1])
      {
        b = true;
        continue;
      }
    }
  } while (b);
}

bool SnakeGame::collisionCheck(int8_t x, int8_t y)
{
  for (int i = 1; i < snake_length; i++)
  {
    if (x == snake[i][0] && y == snake[i][1])
      return true;
  }
  if (x < 0 || y < 0 || x >= MAP_SIZE_X || y >= MAP_SIZE_Y)
    return true;
  return false;
}

void SnakeGame::drawMap()
{
  int offsetMapX = SCREEN_WIDTH - SNAKE_PIECE_SIZE * MAP_SIZE_X - 2;
  int offsetMapY = 2;

  display.drawRect(fruit[0] * SNAKE_PIECE_SIZE + offsetMapX, fruit[1] * SNAKE_PIECE_SIZE + offsetMapY, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_INVERSE);
  display.drawRect(offsetMapX - 2, 0, SNAKE_PIECE_SIZE * MAP_SIZE_X + 4, SNAKE_PIECE_SIZE * MAP_SIZE_Y + 4, SSD1306_WHITE);
  for (int i = 0; i < snake_length; i++)
  {
    display.fillRect(snake[i][0] * SNAKE_PIECE_SIZE + offsetMapX, snake[i][1] * SNAKE_PIECE_SIZE + offsetMapY, SNAKE_PIECE_SIZE, SNAKE_PIECE_SIZE, SSD1306_WHITE);
  }
}

void SnakeGame::drawScore()
{
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 2);
  display.print(F("Score:"));
  display.println(snake_length - STARTING_SNAKE_SIZE);
}

void SnakeGame::drawPressToStart()
{
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 20);
  display.print(F("Press a\n button to\n start the\n game!"));
}

void SnakeGame::drawGameover()
{
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(2, 50);
  display.println(F("GAMEOVER"));
}