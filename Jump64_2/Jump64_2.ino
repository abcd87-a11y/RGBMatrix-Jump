#include <Adafruit_NeoPixel.h>
#include <N64Controller.h>
#include "Hardware.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"

const int helligkeit = 5;
Adafruit_NeoPixel matrix(NUMLEDS, PIXPIN, NEO_RGB + NEO_KHZ800);

//0 = Lueft, 1 = Wand/Boden
const byte level_1[8] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000111,
  B11111111
};
const byte level_2[8] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11100111,
  B11100111
};

const byte level_3[8] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11111111,
  B11111111
};

const byte level_4[8] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00001110,
  B00000011,
  B11100000,
  B00000000
};
const byte level_5[8] = {
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B11001111,
  B00000000,
  B00000000
};
const byte level_6[8] = {
  B01000000,
  B01011111,
  B01000001,
  B01111011,
  B00001001,
  B11101101,
  B00100001,
  B00111111
};
const byte level_7[8] = {
  B00000001,
  B11111101,
  B10000001,
  B10111111,
  B10100001,
  B10101001,
  B10001000,
  B11111111
};

const int MAX_LEVELS = 7;
const byte* world[MAX_LEVELS] = { level_1, level_2, level_3, level_4, level_5, level_6, level_7 };

uint8_t currentLevel = 0;
const byte* currentMap = world[0];
bool isDead = false;

N64Controller n64(12);

Player player(1.f, 6.99f);
Enemy enemy(6.f, 5.99f);
Enemy enemy2(6.f, 4.99f);

void setup() {
  pinMode(JMPBUTTON, INPUT_PULLUP);
  pinMode(RIGHTBUTTON, INPUT_PULLUP);
  pinMode(LEFTBUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  n64.begin();

  digitalWrite(LED, LOW);

  matrix.begin();
  matrix.setBrightness(helligkeit);
  matrix.clear();
  matrix.show();
}

void loop() {
  static unsigned long millis_old = 0;
  unsigned long millis_current = millis();
  unsigned long dt = millis_current - millis_old;
  millis_old = millis_current;

  n64.update();
  player.update(dt, currentMap, n64);
  if (currentLevel == 2) {
    enemy.update(dt, currentMap, n64);
  }
  if (currentLevel == 4) {
    enemy2.update(dt, currentMap, n64);
  }

  if (player.getX() >= 8.f) {
    if (currentLevel < MAX_LEVELS) {
      currentLevel++;
      if (currentLevel > MAX_LEVELS - 1) {
        currentLevel = 0;
      }
      if (currentLevel == 2) {
        enemy.setX(7.99f);
      }
      if (currentLevel == 4) {
        enemy2.setX(7.99f);
      }
      player.setX(0.f);
      currentMap = world[currentLevel];
    } else {
      player.setX(7.99f);
    }
  } else if (player.getX() < 0.f) {
    if (currentLevel > 0) {
      currentLevel--;
      player.setX(7.99f);
      currentMap = world[currentLevel];
    } else {
      player.setX(0.f);
    }
  }

  if (player.getY() >= 8.0f) {
    isDead = true;
  }

  if (currentLevel == 2 || currentLevel == 4) {
    int pX = (int)player.getX();
    int pY = (int)player.getY();
    int eX = 0;
    int eY = 0;
    if (currentLevel == 2) {
      eX = (int)enemy.getX();
      eY = (int)enemy.getY();
    }
    if (currentLevel == 4) {
      eX = (int)enemy2.getX();
      eY = (int)enemy2.getY();
    }

    if (pX == eX && (pY == eY || pY == eY - 1)) {
      isDead = true;
    }
  }
  if (isDead) {
    digitalWrite(LED, HIGH);
    while (true)
      ;
  }

  matrix.clear();
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 8; x++) {
      if (bitRead(currentMap[y], 7 - x)) {
        matrix.setPixelColor((y << 3) | x, 255, 0, 0);
      }
    }
  }
  player.draw(matrix);
  if (currentLevel == 2) {
    enemy.draw(matrix);
  }
  if (currentLevel == 4) {
    enemy2.draw(matrix);
  }
  matrix.show();
}
