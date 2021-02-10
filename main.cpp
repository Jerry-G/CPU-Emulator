#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include "jm.cpp"

using std::cout;
using std::endl;

// the number of pixels used to represent one pixle on the machine
#define SIZE 5
// geometry of your window
#define WIDTH 240 * SIZE
#define HEIGHT 136 * SIZE

void save_texture(const char *file_name, SDL_Renderer *renderer, SDL_Texture *texture)
{
  SDL_Texture *target = SDL_GetRenderTarget(renderer);
  SDL_SetRenderTarget(renderer, texture);
  int width, height;
  //SDL_QueryTexture(texture, NULL, NULL, &width, &height);
  SDL_QueryTexture(target, NULL, NULL, &width, &height);
  //SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
  SDL_Surface *surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
  SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
  IMG_SavePNG(surface, file_name);
  SDL_FreeSurface(surface);
  SDL_SetRenderTarget(renderer, target);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    printf("Usage: ./main rom\n");
    return 1;
  }
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    exit(1);
  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  window = SDL_CreateWindow("SDL",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WIDTH,
                            HEIGHT,
                            0);

  renderer = SDL_CreateRenderer(window, -1, 0);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  bool done = false;
  JM machine = JM();
  machine.load(argv[1]);
  bool red, grn, blu, itn;
  uint8_t color[3];
  //int size = 2;
  SDL_Rect rect;
  //rect.w = size;
  //rect.h = size;
  rect.w = SIZE;
  rect.h = SIZE;
  SDL_Event event;
  int count = 0;
  int frame = 0;

  SDL_Texture *texture;
  while (!done)
  {
    machine.run();

    if (machine.DRAW)
    {
      machine.DRAW = false;
      for (int i = 0; i < 0x1FE0; ++i)
      {
        for (int j = 3; j >= 0; --j)
        {
          machine.nibbletrgb(color, machine.RAM[i] >> (j * 4));

          SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], SDL_ALPHA_OPAQUE);
          int x = (i * 4 + (3 - j)) % 240;
          int y = (i * 4 + (3 - j)) / 240;
          //SDL_RenderDrawPoint(renderer, x, y);
          //rect.x = x * size;
          //rect.y = y * size;
          rect.x = x * SIZE;
          rect.y = y * SIZE;
          SDL_RenderFillRect(renderer, &rect);
        }
      }
      SDL_RenderPresent(renderer);
      SDL_Delay(5);
    }

    //machine.printRegs();

    count++;
    if (machine.HALT)
      break;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        done = true;
    }
  }
  SDL_RenderPresent(renderer);
  //printf("done\n");
  SDL_Delay(2000);
  SDL_RenderCopy(renderer, texture, NULL, NULL);

  //save_texture("brock_lesner.png", renderer, texture);

  if (renderer)
  {
    SDL_DestroyRenderer(renderer);
  }
  if (window)
  {
    SDL_DestroyWindow(window);
  }
  SDL_Quit();
  return 0;
}
