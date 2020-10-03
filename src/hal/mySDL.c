#ifdef __NAVY__

#include <sdl.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

uint8_t *SDL_GetKeyState(int *numkeys) {
  assert(0);
  return NULL;
}

void SDL_SetPalette(SDL_Surface *s, int flags, SDL_Color *colors, int firstcolor, int ncolors) {
  assert(s);
  assert(s->format);
  assert(s->format->palette);
  assert(firstcolor == 0);

  if((s->flags & SDL_HWSURFACE) && s->format->palette->colorsARGB == NULL) {
    s->format->palette->colorsARGB = malloc(sizeof(SDL_Color) * 256);
    assert(s->format->palette->colorsARGB);
  }

  /* Set the new palette. */
  s->format->palette->ncolors = ncolors;
  memcpy(s->format->palette->colors, colors, sizeof(SDL_Color) * ncolors);

  if(s->flags & SDL_HWSURFACE) {
    assert(ncolors == 256);
    for (int i = 0; i < ncolors; i ++) {
      uint8_t r = colors[i].r;
      uint8_t g = colors[i].g;
      uint8_t b = colors[i].b;
      s->format->palette->colorsARGB[i].val = (r << 16) | (g << 8) | b;
    }
    SDL_UpdateRect(s, 0, 0, 0, 0);
  }
}

int SDL_Init(uint32_t flags) {
  return 0;
}

void SDL_Quit(void) {
}

void SDL_PauseAudio(int pause_on) {
}

char *SDL_GetError(void) {
  return "Navy does not support SDL_GetError()";
}

void SDL_LockAudio(void) {
}

void SDL_UnlockAudio(void) {
}

int SDL_ShowCursor(int toggle) {
  return 0;
}

void SDL_CloseAudio(void) {
}

void SDL_WM_SetCaption(const char *title, const char *icon) {
}

#endif
