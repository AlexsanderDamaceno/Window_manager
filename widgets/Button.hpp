#include "./Widget.hpp"

#pragma once


class Button : public Widget {

public:

    using OnClickCallback = std::function<void()>;

    Button(int x, int y, int width, int height, const std::string& label)
        : rect(x, y, width, height), label(label) {}

    void handleEvent(SDL_Event& e) override {

      std::cout << "BUTTON" << std::endl;

      if (e.type == SDL_MOUSEMOTION) {
      isHovered = (e.button.x >= rect.getX() && e.button.x <= rect.getX() + rect.getWidth() &&
                   e.button.y >= rect.getY() && e.button.y <= rect.getY() + rect.getHeight());

                   std::cout <<  "hover "<< isHovered  << std::endl;

}else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT &&
         e.button.x >= rect.getX() && e.button.x <= rect.getX() + rect.getWidth() &&
         e.button.y >= rect.getY() && e.button.y <= rect.getY() + rect.getHeight()) {

         if (onClick) {
             onClick();
         }
     }

    }

    void render(SDL_Renderer* renderer, int windowX, int windowY) override {

        SDL_Color backgroundColor = isHovered ? hoverBackgroundColor : defaultBackgroundColor;


        SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
        SDL_Rect sdlRect = {rect.getX() + windowX, rect.getY() + windowY, rect.getWidth(), rect.getHeight()};
        SDL_RenderFillRect(renderer, &sdlRect);




       renderText(label, rect.getX() + windowX + 5, rect.getY() + windowY + 5, 255, 255, 255 , renderer);
    }

    const Rectangle& getRect() const
    {
        return rect;
    }

    const void setOnClick(std::function<void()> onClickAction)
    {
         onClick = onClickAction;

    }

    void MouseOut() override{
        isHovered = false;
    }



private:
    Rectangle rect;
    std::string label;

    bool isHovered = false;
    SDL_Color hoverBackgroundColor = {0, 0, 255, 255};
    SDL_Color defaultBackgroundColor = {100, 100, 100, 255};

    OnClickCallback onClick;
};
