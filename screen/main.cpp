/*  take a snapshot of machine ram and
 *  display the screen
 *
 */

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>

#define WIDTH 640
#define HEIGHT 400

uint8_t VRAM[320 * 200];

bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer);

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) exit(1);
  SDL_Window* window = NULL;
  SDL_Renderer* renderer = NULL;

  window = SDL_CreateWindow("SDL", 
      SDL_WINDOWPOS_CENTERED, 
      SDL_WINDOWPOS_CENTERED, 
      WIDTH, 
      HEIGHT, 
      0);
      
  renderer = SDL_CreateRenderer(window, -1, 0); 


  SDL_bool done = SDL_FALSE;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  std::string line;
  std::ifstream myfile ("vram");

  //populate vram
  for(int i = 0; i < std::size(VRAM); i++){
    getline(myfile,line);
    VRAM[i] = std::stoi(line,nullptr,16);
  }


  uint8_t color = 0;
  uint8_t red;
  uint8_t grn;
  uint8_t blu;
  int size = 2;
  while (!done) {
    SDL_Event event;

    for(int i = 0; i < std::size(VRAM); ++i){
      red = (VRAM[i] & 0xE0);
      grn = (VRAM[i] & 0x1C) << 3;
      blu = (VRAM[i] & 0x03) << 6;
      SDL_SetRenderDrawColor(renderer, red, grn, blu, SDL_ALPHA_OPAQUE);
      int x = i%320;
      int y = i/320;
      //SDL_RenderDrawPoint(renderer, x, y);
      SDL_Rect rect;
      rect.x = x*size;
      rect.y = y*size;
      rect.w = size;
      rect.h = size;
      SDL_RenderFillRect(renderer, &rect);
    }
    SDL_RenderPresent(renderer);

    /*
    for(int i = 0; i < 2*2*2*2*size; i+=size) {
      for(int j = 0; j < 2*2*2*2*size; j+=size) {
        red = (color & 0xE0);
        grn = (color & 0x1C) << 3;
        blu = (color & 0x03) << 6;
        SDL_SetRenderDrawColor(renderer, red, grn, blu, SDL_ALPHA_OPAQUE);
        SDL_Rect rect;
        rect.x = i;
        rect.y = j;
        rect.w = size;
        rect.h = size;
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        color++;
        SDL_Delay(5);
      }
    }
    */


    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) done = SDL_TRUE;
    }
    SDL_Delay(5);
  }

  if (renderer) { SDL_DestroyRenderer(renderer); }
  if (window) { SDL_DestroyWindow(window); }
  SDL_Quit();
  return 0;
}

bool saveScreenshotBMP(std::string filepath, SDL_Window* SDLWindow, SDL_Renderer* SDLRenderer) {
  SDL_Surface* saveSurface = NULL;
  SDL_Surface* infoSurface = NULL;
  infoSurface = SDL_GetWindowSurface(SDLWindow);
  if (infoSurface == NULL) {
    std::cerr << "Failed to create info surface from window in saveScreenshotBMP(string), SDL_GetError() - " << SDL_GetError() << "\n";
  } else {
    unsigned char * pixels = new (std::nothrow) unsigned char[infoSurface->w * infoSurface->h * infoSurface->format->BytesPerPixel];
    if (pixels == 0) {
      std::cerr << "Unable to allocate memory for screenshot pixel data buffer!\n";
      return false;
    } else {
      if (SDL_RenderReadPixels(SDLRenderer, &infoSurface->clip_rect, infoSurface->format->format, pixels, infoSurface->w * infoSurface->format->BytesPerPixel) != 0) {
        std::cerr << "Failed to read pixel data from SDL_Renderer object. SDL_GetError() - " << SDL_GetError() << "\n";
        delete[] pixels;
        return false;
      } else {
        saveSurface = SDL_CreateRGBSurfaceFrom(pixels, infoSurface->w, infoSurface->h, infoSurface->format->BitsPerPixel, infoSurface->w * infoSurface->format->BytesPerPixel, infoSurface->format->Rmask, infoSurface->format->Gmask, infoSurface->format->Bmask, infoSurface->format->Amask);
        if (saveSurface == NULL) {
          std::cerr << "Couldn't create SDL_Surface from renderer pixel data. SDL_GetError() - " << SDL_GetError() << "\n";
          delete[] pixels;
          return false;
        }
        SDL_SaveBMP(saveSurface, filepath.c_str());
        SDL_FreeSurface(saveSurface);
        saveSurface = NULL;
      }
      delete[] pixels;
    }
    SDL_FreeSurface(infoSurface);
    infoSurface = NULL;
  }
  return true;
}

