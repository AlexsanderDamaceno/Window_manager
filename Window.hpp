#pragma once

#include "./widgets/Widget.hpp"

class Window {
public:
    Window(int x, int y, int width, int height,  const SDL_Color& color,  const std::string& title)
        : rect(x, y, width, height), color(color), title(title) {}


    bool operator==(const Window& other) const
    {
            return this == &other;
    }


    void addWidget(Widget* widget) {
        widgets.push_back(widget);
    }

    void removeWidget(Widget* widget) {
        widgets.erase(std::remove(widgets.begin(), widgets.end(), widget), widgets.end());
    }

    void renderWidgets(SDL_Renderer* renderer , int windowx , int windowy)
    {
        for (Widget* widget : widgets) {
            widget->render(renderer , windowx , windowy);
        }
    }

    const Rectangle& getRect() const {
        return rect;
    }

    void updatePosition(int x, int y) {
        rect = Rectangle(x, y, rect.getWidth(), rect.getHeight());
    }

    void renderDecoration(SDL_Renderer* renderer) {

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_Rect titleBarRect = {rect.getX(), rect.getY(), rect.getWidth(), TITLE_BAR_HEIGHT};
        SDL_RenderFillRect(renderer, &titleBarRect);


        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_Rect closeButtonRect = {rect.getX() + rect.getWidth() - CLOSE_BUTTON_SIZE - BORDER_SIZE,
                                     rect.getY() + BORDER_SIZE, CLOSE_BUTTON_SIZE, CLOSE_BUTTON_SIZE};
        SDL_RenderFillRect(renderer, &closeButtonRect);
    }

    void handleMouseDown(SDL_Event e) {

      int x =  e.button.x;
      int y =  e.button.y;

      bool has_selected_widget = false;


      if (x >= rect.getX() && x <= rect.getX() + BORDER_SIZE &&
        y >= rect.getY() && y <= rect.getY() + BORDER_SIZE) {

        isResizing = true;
        resizeHandle = 1;

      } else if (x >= rect.getX() && x <= rect.getX() + rect.getWidth() &&
           y >= rect.getY() && y <= rect.getY() + BORDER_SIZE) {

        isResizing = true;
        resizeHandle = 2;

      }else {

      for (Widget* widget : widgets) {

          if (widget->getRect().contains(x - rect.getX(), y - rect.getY())) {

              selectedWidget = widget;


              has_selected_widget = true;
              break;
          }

      }


      if(!has_selected_widget)
      {
        selectedWidget = nullptr;
      }else{

        e.button.x -= rect.getX();
        e.button.y -= rect.getY();
        selectedWidget->handleEvent(e);
      }

    //  selectedWidget->handleEvent(e);
    }

      // Check if the title bar is clicked for window dragging
    //  if (y < rect.getY() + TITLE_BAR_HEIGHT) {
    //      isDragging = true;
    //      offsetX = x - rect.getX();
    //      offsetY = y - rect.getY();
  //    }

  }

 void  handleMouseMove(SDL_Event e)
 {

   int x = e.motion.x;
   int y = e.motion.y;


    if (isResizing) {
        int newWidth = rect.getWidth();
        int newHeight = rect.getHeight();

        // Adjust the window size based on the resizing handle
        switch (resizeHandle) {
            case 1: // Top-left
                newWidth = rect.getWidth() + rect.getX() - x;
                newHeight = rect.getHeight() + rect.getY() - y;
                break;
            case 2: // Top
                newHeight = rect.getHeight() + rect.getY() - y;
                break;
            // Add similar cases for other resizing handles
        }

        // Limit the minimum size of the window
        //newWidth = std::max(newWidth, MIN_WINDOW_WIDTH);
        //newHeight = std::max(newHeight, MIN_WINDOW_HEIGHT);

        rect = Rectangle(x, y, newWidth, newHeight);
    } else {
        // Handle other mouse move logic if resizing is not occurring


        // pass mouse move event to widget

        int x = e.button.x;
        int y = e.button.y;


        int is_widget_hovered = false;

        for (Widget* widget : widgets)
        {



            if (widget->getRect().contains(x , y )) {
              std::cout << "teste" << std::endl;

            //    e.button.x = rect.getX();
              //  e.button.y = rect.getY();

            //    std::cout << "mouse x:" << x << "mouse y: " << y << "widget x:" << widget->getRect().getX() << std::endl;
                HoveredWidget = widget;
                is_widget_hovered = true;
                widget->handleEvent(e);
                break;
            }

        }

        if(!is_widget_hovered and HoveredWidget){
           HoveredWidget->MouseOut();

           HoveredWidget = nullptr; 
        }

    }

    return;

}

void handleMouseUp() {
    isResizing = false;
    resizeHandle = 0;

}

  void handleKeyDown(SDL_Event &e)
  {


    std::cout << "TESTE 2" << std::endl;
    if(selectedWidget)
        selectedWidget->handleEvent(e);

    return;
 }



    Rectangle rect;
    SDL_Color color;
    std::string title;
    std::vector<Widget*> widgets;

    bool isDragging = 0;
    bool isResizing = false;
    int resizeHandle = 0;



private:

  Widget * selectedWidget = nullptr;
  Widget * HoveredWidget  = nullptr;

};
