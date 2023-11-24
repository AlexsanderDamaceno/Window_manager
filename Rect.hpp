#pragma once 

class Rectangle {
public:
    Rectangle(int x, int y, int width, int height)
        : x(x), y(y), width(width), height(height) {}

    bool contains(int px, int py) const {
        return px >= x && px <= x + width && py >= y && py <= y + height;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

private:
    int x;
    int y;
    int width;
    int height;
};
