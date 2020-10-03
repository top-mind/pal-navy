#ifndef _MYSDL_H
#define _MYSDL_H

#ifdef __NAVY__
#include "sdl.h"  // actually provided by NDL

#define SDL_SwapLE32(x) (x)
#define SDL_SwapLE16(x) (x)
#define SDL_strcasecmp  strcasecmp
#define SDL_strncasecmp strncasecmp

typedef int SDL_AudioFormat;
typedef int SDL_AudioDeviceID;

#else
#include "SDL.h"
#endif

#endif
