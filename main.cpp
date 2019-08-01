#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream>
#include "jm.cpp"

using std::cout;
using std::endl;

#define WIDTH 480
#define HEIGHT 272

int main(int argc, char* argv[]) {
  if(argc != 2) {
    printf("Usage: ./main rom\n");
    return 1;
  }
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



  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);
  bool done = false;
  JM machine = JM();
  machine.load(argv[1]);
  bool red, grn, blu, itn;
  uint8_t color[3];
  int size = 2;
  SDL_Rect rect;
  rect.w = size;
  rect.h = size;
  SDL_Event event;
  int count = 0;
  //machine.printMem(0x1FEA);
    
  while (!done) 
  {
    machine.run();

    if(machine.DRAW)
    {
      machine.DRAW = false;
      for(int i=0; i<0x1FE0; ++i){
        for(int j=3; j>=0; --j){
          red = ((machine.RAM[i] >> (j*4)) & 0x8);
          grn = ((machine.RAM[i] >> (j*4)) & 0x4);
          blu = ((machine.RAM[i] >> (j*4)) & 0x2);
          itn = ((machine.RAM[i] >> (j*4)) & 0x1);

          /* rgbi to rgb color converter */
          if( !red && !grn && !blu && itn){
            color[0] = 192;
            color[1] = 192;
            color[2] = 192;
          }else{
            color[0] = (red)?255:0; color[1] = (grn)?255:0; color[2] = (blu)?255:0;
            if(itn){ color[0] /= 2; color[1] /= 2; color[2] /= 2; }
          }
          //machine.rgbitrgb(&color, red, grn, blu, itn); /* fix this function */
          
          
          SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], SDL_ALPHA_OPAQUE);
          int x = (i*4+(3-j))%240;
          int y = (i*4+(3-j))/240;
          //SDL_RenderDrawPoint(renderer, x, y);
          rect.x = x*size;
          rect.y = y*size;
          SDL_RenderFillRect(renderer, &rect);

        }
      }
      SDL_RenderPresent(renderer);
      SDL_Delay(5);
    }

    machine.printRegs();

    count++;
    if(machine.HALT) break;

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) done = true;
    }
  }
  SDL_RenderPresent(renderer);
  //printf("done\n");
  SDL_Delay(2000);

  if (renderer) { SDL_DestroyRenderer(renderer); }
  if (window) { SDL_DestroyWindow(window); }
  SDL_Quit();
  return 0;
}
