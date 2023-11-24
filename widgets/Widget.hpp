#pragma once

class Widget {
public:
    virtual void handleEvent(SDL_Event& e) = 0;
    virtual void render(SDL_Renderer* renderer, int windowX, int windowY) = 0;
    virtual ~Widget() = default;
    virtual const Rectangle& getRect() const = 0;



private:

};
