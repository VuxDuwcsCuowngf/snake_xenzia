#include <bmps.h>
#include<PCD8544_HOANGSA.h>

#define RST 22     // lcd nokia is screen
#define CE 24
#define DC 26
#define DIN 28
#define CLK 30

#define UP A3       //button to click
#define DOWN A2
#define RIGHT A0
#define LEFT A1

PCD8544 lcd (RST, CE, DC, DIN, CLK);

typedef struct
{
  int stt;
  int x;
  int y;
} snake;            // A block of snake
int size = 2;       // a block has size 3x3 pixel

snake snakes[100], new_pos;  // Max len is 100

int prey_x = random(0, 84/size);
int prey_y = random(0, 48/size);


int score = 0;
int direct = 1;
int game_stt = 0;

void setup()
{
  lcd.ON();
  lcd.SET(50, 0, 0, 0, 4); // contrast and bias

  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(RIGHT, INPUT);

  // 0-> tail ...... i-> head
  // first block of snake -- tail
  snakes[0].stt = 1;   // blcok status; if 0 -> don't print, 1 -> print
  snakes[0].x = 20;
  snakes[0].y = 20;
  snakes[1].stt = 1;   // blcok status; if 0 -> don't print, 1 -> print
  snakes[1].x = 22;
  snakes[1].y = 20;
  snakes[2].stt = 1;   // blcok status; if 0 -> don't print, 1 -> print
  snakes[2].x = 24;
  snakes[2].y = 20;
}

void loop()
{
  START:
  if (game_stt == 0)
  {
    game_stt = 1;
    delay(1000);
  }

  int u = analogRead(UP);
  int d = analogRead(DOWN);
  int r = analogRead(RIGHT);
  int l = analogRead(LEFT);

  if (u > 500 && direct != -1)
  {
    direct = 1;
  }
  if (d > 500 && direct != 1)
  {
    direct = -1;
  }
  if (r > 500 && direct != -2)
  {
    direct = 2;
  }
  if (l > 500 && direct != 2)
  {
    direct = -2;
  }

  int len = getLengthSnake();
  if (direct == -2)
  {
    new_pos.stt = 1;
    new_pos.x = snakes[len-1].x - size;
    new_pos.y = snakes[len-1].y; 
    if (new_pos.x == 0) new_pos.x = 82;
  }
  if (direct == 2)
  {
    new_pos.stt = 1;
    new_pos.x = snakes[len-1].x + size;
    new_pos.y = snakes[len-1].y;
    if (new_pos.x == 84) new_pos.x = 0;
  }
  if (direct == -1)
  {
    new_pos.stt = 1;
    new_pos.y = snakes[len-1].y + size;
    new_pos.x = snakes[len-1].x;
    if (new_pos.y == 48) new_pos.y = 0;
  }
  if (direct == 1)
  {
    new_pos.stt = 1;
    new_pos.y = snakes[len-1].y - size;
    new_pos.x = snakes[len-1].x;
    if (new_pos.y == 0) new_pos.y = 46;
  }

  snakes[len] = new_pos;

  if (new_pos.x == prey_x*size && new_pos.y == prey_y*size)
  {
    prey_x = random(0, 84/size);
    prey_y = random(0, 48/size);
    score ++;
  }
  else
  {
    delSnake(snakes, getLengthSnake());
  }

  lcd.FillRect(prey_x*size, prey_y*size, size, size, 1);
  
  for(int i=0; i<getLengthSnake(); i++)
  {
    lcd.FillRect(snakes[i].x, snakes[i].y, size, size, 1);
    lcd.FillRect(snakes[0].x, snakes[0].y, size, size, 0);
  }
  lcd.Number_Long(1, 1, score, 0, 1);

  len = getLengthSnake();
  for (int i=0; i<len-1; i++)
  {
    if (snakes[i].x == snakes[len-1].x && snakes[i].y == snakes[len-1].y)
    {
      delay(3000);
      lcd.Clear();
      goto START;
    }
  }

  delay(80);
  lcd.Display();
}

int getLengthSnake()
{
  for (int i = 0; i < 100; i++)
  {
    if (snakes[i].stt == 0)
    {
      return (i);
    }
  }
}

void delSnake(snake snakes[], int n) {
  for (int i = 0; i < n; i++) 
  {
    snakes[i] = snakes[i + 1];
  }
}
