//#include "sdl2/include/SDL.h"
//#include "SDL/SDL.h"
#ifdef SDL2
unsigned char sdlkeymap[SDL_NUM_SCANCODES];
#else
unsigned char sdlkeymap[SDLK_LAST];
#endif
void InitKeyMap();
