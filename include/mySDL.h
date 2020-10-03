#ifndef _MYSDL_H
#define _MYSDL_H

#ifdef __NAVY__
#include "sdl.h"  // actually provided by NDL

#define SDL_SwapLE32(x) (x)
#define SDL_SwapLE16(x) (x)
#define SDL_strncasecmp strncasecmp

typedef int SDL_AudioFormat;
typedef int SDL_AudioDeviceID;

#define SDL_MUSTLOCK(screen) 0

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, int firstcolor, int ncolors);
int SDL_Init(uint32_t flags);
void SDL_Quit(void);
void SDL_PauseAudio(int pause_on);
char *SDL_GetError(void);
void SDL_LockAudio(void);
void SDL_UnlockAudio(void);
int SDL_ShowCursor(int toggle);
void SDL_CloseAudio(void);
void SDL_WM_SetCaption(const char *title, const char *icon);

#else
#include "SDL.h"
#endif

#endif
