#pragma once
#include "GameObject.h"
#include "Arduino.h"

#include "Hardware.h"
//#include <N64Controller.h>

class Player : public GameObject {
private:
  float speed = 0.0072f;
  float velocityY = 0.f;
  float gravity = 0.00005;
  float jumpForce = -0.021f;
  bool isGrounded = true;

public:
  Player(float startX, float startY)
    : GameObject(startX, startY) {}

  void update(unsigned long dt, const byte *levelMap, N64Controller &n64) override {
    float oldX = x;
    //Laufen
    if (!digitalRead(RIGHTBUTTON) || n64.axis_x() > 20) x += speed * dt;

    if (!digitalRead(LEFTBUTTON) || n64.axis_x() < -20) x -= speed * dt;

    int px = (int)x;
    int py = (int)y;

    //X-Kollision
    if (px >= 0 && px <= 7 && py >= 0 && py <= 7) {
      if (bitRead(levelMap[py], 7 - px)) {  //Wand getroffen
        x = oldX;
        px = (int)x;
      }
    }
    
    //Y-Kollision
    velocityY += gravity * dt;
    y += velocityY * dt;

    if (y < 0.f) {
      y = 0.f;
    }
    py = (int)y;
    isGrounded = false;
    if (px >= 0 && px <= 7 && py >= 0 && py <= 7) {
      if (bitRead(levelMap[py], 7 - px)) {  //Boden / Plattform getroffen
        if (velocityY > 0) {
          y = (float)py - 0.01f;
          isGrounded = true;
        } else if (velocityY < 0) {
          y = (float)py + 1.f;
        }
        velocityY = 0.f;
      }
    }
    //Springen
    if ((!digitalRead(JMPBUTTON) || n64.A()) && isGrounded) {
      isGrounded = false;
      velocityY = jumpForce;
    }
  }

  void draw(Adafruit_NeoPixel &mat) override {
    mat.setPixelColor(getIndex((int)x, (int)y), 0, 255, 0);
  }
};