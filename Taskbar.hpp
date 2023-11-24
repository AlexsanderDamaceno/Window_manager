#pragma once

#include "./Config.hpp"
#include "./widgets/Widget.hpp"
#include "./Window.hpp"

#include <vector>

class Taskbar {
public:
    Taskbar(SDL_Renderer* renderer) : renderer(renderer) {}

    void addTask(Window& window) {
        tasks.push_back(window);
    }

    void removeTask(Window& window) {

   //  tasks.erase(std::remove_if(tasks.begin(), tasks.end(),
    //        [&Window]( Window& r) { return &r == &Window; }), tasks.end());
    //    taskbar.removeTask(rect);

       // tasks.erase(std::remove(tasks.begin(), tasks.end(), window), tasks.end());
    }

    void render() {
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_Rect taskbarRect = {0, SCREEN_HEIGHT - TASKBAR_HEIGHT, SCREEN_WIDTH, TASKBAR_HEIGHT};
        SDL_RenderFillRect(renderer, &taskbarRect);

        int taskX = 0;

        /*
        for (auto& task : this->tasks) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect taskRect = {taskX, SCREEN_HEIGHT - TASKBAR_HEIGHT, TASKBAR_HEIGHT, TASKBAR_HEIGHT};
            SDL_RenderFillRect(renderer, &taskRect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Rect titleRect = {taskX + BORDER_SIZE, SCREEN_HEIGHT - TASKBAR_HEIGHT + BORDER_SIZE,
                                  TASKBAR_HEIGHT - 2 * BORDER_SIZE, TASKBAR_HEIGHT - 2 * BORDER_SIZE};
            SDL_RenderFillRect(renderer, &titleRect);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &titleRect);

            taskX += TASKBAR_HEIGHT;


            renderText(task.title, titleRect.x + 5, titleRect.y + 5, 255, 255, 255);


        }

        */
    }

    void renderText( std::string& text, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
        SDL_Color color = {r, g, b};
        SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        int width, height;
        SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

        SDL_Rect destRect = {x, y, width, height};
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    ~Taskbar() {
        TTF_CloseFont(font);
    }

private:
    SDL_Renderer* renderer;
    std::vector<Window> tasks;
    TTF_Font* font = TTF_OpenFont("Arial.ttf", 12);
};
