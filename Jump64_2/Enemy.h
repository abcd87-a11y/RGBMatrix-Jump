#pragma once
#include "GameObject.h"
#include "Arduino.h"

class Enemy : public GameObject {
private:
  float speed = 0.0025f;
  int direction = 1;

public:
  Enemy(float startX, float startY)
    : GameObject(startX, startY) {}

  void update(unsigned long dt, const byte *levelMap, N64Controller &n64) override {
    float oldX = x;

    x += speed * dt * direction;

    int px = (int)x;
    int py = (int)y;

    if (px >= 8) {
      x = 7.99f;
      direction = -1;
      px = (int)x;
    } else if (px < 0) {
      x = 0.f;
      direction = 1;
      px = (int)x;
    }

    if (px >= 0 && px <= 7 && py >= 0 && py <= 7) {
      if (bitRead(levelMap[py], 7 - px)) {
        x = oldX;
        direction *= -1;
      } else if (py + 1 <= 7) {
        if (bitRead(levelMap[py + 1], 7 - px) == 0) {
          x = oldX;
          direction *= -1;
        }
      }
    }
  }

  void draw(Adafruit_NeoPixel &mat) override {
    int px = (int)x;
    int py = (int)y;
    mat.setPixelColor(getIndex(px, py), 0, 0, 255);

    if (py - 1 >= 0) {
      mat.setPixelColor(getIndex(px, py - 1), 0, 0, 255);
    }
  }
};