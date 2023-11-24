#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>


#include "./Config.hpp"
#include "./Rect.hpp"
#include "./widgets/Widget.hpp"
#include "./widgets/TextBox.hpp"
#include "./Taskbar.hpp"
#include "./Window.hpp"






class WindowManager {
public:
    WindowManager() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        if (TTF_Init() < 0) {
            std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        window = SDL_CreateWindow("Window Manager", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
        if (renderer == nullptr) {
            std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
            exit(EXIT_FAILURE);
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
    }

    void removeWindow(Window& window) {

       windows.erase(std::remove_if(windows.begin(), windows.end(),
            [&window](Window& r) { return &r == &window; }), windows.end());

         // taskbar.removeTask(rect);
    }

    void renderBackground(const std::string& imagePath) {
       SDL_Surface* backgroundSurface = IMG_Load(imagePath.c_str());

       if (backgroundSurface == nullptr) {
           printf("Unable to load background image! SDL_image Error: %s\n", IMG_GetError());
           return;
       }

       SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
       if (backgroundTexture == nullptr) {
           printf("Unable to create texture from background image! SDL Error: %s\n", SDL_GetError());
           SDL_FreeSurface(backgroundSurface);
           return;
       }

       SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

       SDL_DestroyTexture(backgroundTexture);
       SDL_FreeSurface(backgroundSurface);
   }

    void run() {
        bool quit = false;
        SDL_Event e;
        Taskbar taskbar(renderer);

        Window window1(SCREEN_WIDTH / 2 - 200 ,  SCREEN_HEIGHT/2 - 800, 800, 600, {255, 255, 255, 255}, "Window 1");
        TextBox textBox1(40, 50, 300, 40);
        window1.addWidget(&textBox1);

        windows.push_back(window1);


        Window window2(SCREEN_WIDTH / 2,  SCREEN_HEIGHT/2, 800, 600, {255, 255, 255, 255}, "Window 2");
        TextBox textBox2(40, 50, 300, 40);
        window2.addWidget(&textBox2);


        windows.push_back(window2);

      //  taskbar.addTask(window1);
      //  taskbar.addTask(window2);

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }else if (e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN) {
                    std::cout << "TESTE" << std::endl;
                    handleKeyDown(e);
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        handleMouseDown(e.button.x, e.button.y);
                    }
                } else if (e.type == SDL_MOUSEBUTTONUP) {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        handleMouseUp();
                    }
                } else if (e.type == SDL_MOUSEMOTION) {
                    handleMouseMove(e.motion.x, e.motion.y);
                }
            }

            update();
            render(taskbar);
        }
    }

    ~WindowManager() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
    }

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Window* selectedWindow = nullptr;
    Window* last_selectedWindow = nullptr;
    TextBox* selectedTextBox = nullptr;
    const int CLOSE_BUTTON_SIZE = 20;
    int offsetX, offsetY;



    void handleMouseDown(int x, int y) {


    bool has_selected_window = false;

    for (auto& window : windows) {

        if (window.getRect().contains(x, y)) {
            last_selectedWindow = selectedWindow = &window;

            has_selected_window = true;


            // Check for the title bar area (adjust as needed)
            if (y < window.getRect().getY() + TITLE_BAR_HEIGHT)
            {
                selectedWindow->isDragging = true;
                offsetX = x - window.getRect().getX();
                offsetY = y - window.getRect().getY();

                if (window.getRect().contains(x, y) &&
			    x > window.getRect().getX() + window.getRect().getWidth() - CLOSE_BUTTON_SIZE - BORDER_SIZE &&
			    x < window.getRect().getX() + window.getRect().getWidth() - BORDER_SIZE &&
			    y > window.getRect().getY() + BORDER_SIZE &&
			    y < window.getRect().getY() + BORDER_SIZE + CLOSE_BUTTON_SIZE) {
			    // Close the window (you may want to handle this differently)
			    removeWindow(window);


        }else{


            auto it = std::find(windows.begin(), windows.end(), window);
            if (it != windows.end()) {
                std::rotate(it, it + 1, windows.end());
            }

          }


            }//else {
              //  selectedWindow = nullptr;  // Reset selected window if not in the title bar
                // Handle other mouse down logic if needed
          //  }


            if (window.getRect().contains(x, y))
            {
                 window.handleMouseDown(x, y);
                 break;
            }




        }
    }

    if(!has_selected_window){
       last_selectedWindow = nullptr;
    }

    /*
        for (auto& window : windows) {
            if (window.getRect().contains(x, y)) {
                selectedWindow = &window;

                for (Widget* widget : selectedWindow->widgets) {
                    TextBox* textBox = dynamic_cast<TextBox*>(widget);
                    if (textBox && textBox->getRect().contains(x, y)) {
                        selectedTextBox = textBox;
                    }
                }

                // Handle other mouse down logic if needed
            }
        }

        */
    }

    void handleMouseUp() {


       if(selectedWindow && selectedWindow->isResizing)
          selectedWindow->handleMouseUp();

        selectedWindow = nullptr;
        selectedTextBox = nullptr;
    }

    void handleKeyDown(SDL_Event &e) {

      if (last_selectedWindow) {

           last_selectedWindow->handleKeyDown(e);
      }

    }


    void handleMouseMove(int x, int y) {


           if(selectedWindow && selectedWindow->isResizing){
                  selectedWindow->handleMouseMove(x ,y);
                }else if (selectedWindow && selectedWindow->isDragging) {

        selectedWindow->updatePosition(x - offsetX, y - offsetY);

        // Update widget positions as well if needed
        for (Widget* widget : selectedWindow->widgets) {
            if (TextBox* textBox = dynamic_cast<TextBox*>(widget)) {
               // textBox->updatePosition(selectedWindow->getRect().getX() + BORDER_SIZE,
                  //                       selectedWindow->getRect().getY() + TITLE_BAR_HEIGHT + BORDER_SIZE);
            }
        }
    }

    }

    void handleEvent(SDL_Event& e) {
        if (selectedTextBox) {
            selectedTextBox->handleEvent(e);
        }
    }

    void update() {
        // Add any update logic if needed
    }

    void render(Taskbar& taskbar)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);


        renderBackground("./background_img/background_img.jpg");

        for (auto& window : windows) {

            SDL_SetRenderDrawColor(renderer, window.color.r, window.color.g, window.color.b, window.color.a);

            SDL_Rect windowRect = {window.getRect().getX(), window.getRect().getY(), window.getRect().getWidth(), window.getRect().getHeight()};
            SDL_RenderFillRect(renderer, &windowRect);

            window.renderDecoration(renderer);
            window.renderWidgets(renderer , windowRect.x , windowRect.y);
        }

      //  taskbar.render();

        SDL_RenderPresent(renderer);
    }



private:

    std::vector<Window> windows;

    Widget *selectedwidget = nullptr;
};

int main(int argc, char* args[]) {
    WindowManager windowManager;
    windowManager.run();

    return 0;
}
