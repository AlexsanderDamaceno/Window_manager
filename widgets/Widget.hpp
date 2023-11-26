#pragma once

class Widget {
public:
    virtual void handleEvent(SDL_Event& e) = 0;
    virtual void render(SDL_Renderer* renderer, int windowX, int windowY) = 0;
    virtual ~Widget() = default;
    virtual const Rectangle& getRect() const = 0;

    virtual void  MouseOut()
    {

    };







private:



};

void renderText( std::string& text, int x, int y, Uint8 r, Uint8 g, Uint8 b , SDL_Renderer* renderer)
{

   TTF_Font *font = TTF_OpenFont("./Font/Arial.ttf", 20);


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
