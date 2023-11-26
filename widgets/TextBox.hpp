#include "./Widget.hpp"

#pragma once

class TextBox : public Widget {
public:
    TextBox(int x, int y, int width, int height)
        : rect(x, y, width, height), text("") {
           font = TTF_OpenFont("./Font/Arial.ttf", 20);
        }

    void handleEvent(SDL_Event& e) override {


        if (e.type == SDL_TEXTINPUT) {
            text += e.text.text;
        } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && !text.empty()) {
            text.pop_back();
        }
    }

    std::string GetText()
    {
        return text;
    }

    void render(SDL_Renderer* renderer , int windowX , int windowY) override {


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_Rect sdlRect = {rect.getX() + windowX, rect.getY() + windowY, rect.getWidth(), rect.getHeight()};
        SDL_RenderDrawRect(renderer, &sdlRect);


        renderText(text, rect.getX() + windowX + 5, rect.getY() + windowY + 5, 0, 0, 0 , renderer);
    }

    const Rectangle& getRect() const {
        return rect;
    }

    Rectangle rect;
    std::string text;

private:


    void renderText( std::string& text, int x, int y, Uint8 r, Uint8 g, Uint8 b , SDL_Renderer* renderer) {


        if(text.size() == 0)
           return;


        SDL_Color color = {r, g, b};
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

        if (surface == nullptr) {
            std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return;
        }



        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        if (texture == nullptr) {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surface);
            return;
        }


        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

        SDL_Rect destRect = {x, y, width, height};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);

    }

    TTF_Font* font = nullptr;
};
