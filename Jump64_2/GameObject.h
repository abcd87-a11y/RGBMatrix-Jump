#pragma once
class GameObject {
protected:
  float x, y;

public:
  GameObject(float startX, float startY)
    : x(startX), y(startY) {}

  int getIndex(int posX, int posY) {
    return (posY << 3) | posX;
  }

  virtual void update(unsigned long dt, const byte* levelMap, N64Controller &n64) = 0;
  virtual void draw(Adafruit_NeoPixel &mat) = 0;

  float getX() {
    return x;
  }
  float getY() {
    return y;
  }

  void setX(float newX) {
    x = newX;
  }

  void setY (float newY){
    y = newY;
  }
};