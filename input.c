/* -*- mode: c; tab-width: 4; c-basic-offset: 4; c-file-style: "linux" -*- */
//
// Copyright (c) 2009-2011, Wei Mingzhi <whistler_wmz@users.sf.net>.
// Copyright (c) 2011-2020, SDLPAL development team.
// All rights reserved.
//
// This file is part of SDLPAL.
//
// SDLPAL is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "main.h"
#include <math.h>

volatile PALINPUTSTATE   g_InputState;


BOOL                     g_fUseJoystick = TRUE;

static void _default_init_filter() {}
static int _default_input_event_filter(const SDL_Event *event, volatile PALINPUTSTATE *state) { return 0; }
static void _default_input_shutdown_filter() {}

static void (*input_init_filter)() = _default_init_filter;
static int (*input_event_filter)(const SDL_Event *, volatile PALINPUTSTATE *) = _default_input_event_filter;
static void (*input_shutdown_filter)() = _default_input_shutdown_filter;

static const int g_KeyMap[][2] = {
   { SDLK_UP,        kKeyUp },
   { SDLK_KP_8,      kKeyUp },
   { SDLK_DOWN,      kKeyDown },
   { SDLK_KP_2,      kKeyDown },
   { SDLK_LEFT,      kKeyLeft },
   { SDLK_KP_4,      kKeyLeft },
   { SDLK_RIGHT,     kKeyRight },
   { SDLK_KP_6,      kKeyRight },
   { SDLK_ESCAPE,    kKeyMenu },
   { SDLK_INSERT,    kKeyMenu },
   { SDLK_LALT,      kKeyMenu },
   { SDLK_RALT,      kKeyMenu },
   { SDLK_KP_0,      kKeyMenu },
   { SDLK_RETURN,    kKeySearch },
   { SDLK_SPACE,     kKeySearch },
   { SDLK_KP_ENTER,  kKeySearch },
   { SDLK_LCTRL,     kKeySearch },
   { SDLK_PAGEUP,    kKeyPgUp },
   { SDLK_KP_9,      kKeyPgUp },
   { SDLK_PAGEDOWN,  kKeyPgDn },
   { SDLK_KP_3,      kKeyPgDn },
   { SDLK_HOME,      kKeyHome },
   { SDLK_KP_7,      kKeyHome },
   { SDLK_END,       kKeyEnd },
   { SDLK_KP_1,      kKeyEnd },
   { SDLK_r,         kKeyRepeat },
   { SDLK_a,         kKeyAuto },
   { SDLK_d,         kKeyDefend },
   { SDLK_e,         kKeyUseItem },
   { SDLK_w,         kKeyThrowItem },
   { SDLK_q,         kKeyFlee },
   { SDLK_f,         kKeyForce },
   { SDLK_s,         kKeyStatus }
};

static VOID
PAL_KeyDown(
   INT         key,
   BOOL        fRepeat
)
/*++
  Purpose:

    Called when user pressed a key.

  Parameters:

    [IN]  key - keycode of the pressed key.

  Return value:

    None.

--*/
{
   switch (key)
   {
   case kKeyUp:
      if (g_InputState.dir != kDirNorth && !fRepeat)
      {
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirNorth;
      }
      g_InputState.dwKeyPress |= kKeyUp;
      break;

   case kKeyDown:
      if (g_InputState.dir != kDirSouth && !fRepeat)
      {
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirSouth;
      }
      g_InputState.dwKeyPress |= kKeyDown;
      break;

   case kKeyLeft:
      if (g_InputState.dir != kDirWest && !fRepeat)
      {
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirWest;
      }
      g_InputState.dwKeyPress |= kKeyLeft;
      break;

   case kKeyRight:
      if (g_InputState.dir != kDirEast && !fRepeat)
      {
         g_InputState.prevdir = (gpGlobals->fInBattle ? kDirUnknown : g_InputState.dir);
         g_InputState.dir = kDirEast;
      }
      g_InputState.dwKeyPress |= kKeyRight;
      break;

   default:
      g_InputState.dwKeyPress |= key;
      break;
   }
}

static VOID
PAL_KeyUp(
   INT         key
)
/*++
  Purpose:

    Called when user released a key.

  Parameters:

    [IN]  key - keycode of the released key.

  Return value:

    None.

--*/
{
   switch (key)
   {
   case kKeyUp:
      if (g_InputState.dir == kDirNorth)
      {
         g_InputState.dir = g_InputState.prevdir;
      }
      g_InputState.prevdir = kDirUnknown;
      break;

   case kKeyDown:
      if (g_InputState.dir == kDirSouth)
      {
         g_InputState.dir = g_InputState.prevdir;
      }
      g_InputState.prevdir = kDirUnknown;
      break;

   case kKeyLeft:
      if (g_InputState.dir == kDirWest)
      {
         g_InputState.dir = g_InputState.prevdir;
      }
      g_InputState.prevdir = kDirUnknown;
      break;

   case kKeyRight:
      if (g_InputState.dir == kDirEast)
      {
         g_InputState.dir = g_InputState.prevdir;
      }
      g_InputState.prevdir = kDirUnknown;
      break;

   default:
      break;
   }
}

static VOID
PAL_UpdateKeyboardState(
   VOID
)
/*++
  Purpose:

    Poll & update keyboard state.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   static DWORD   rgdwKeyLastTime[sizeof(g_KeyMap) / sizeof(g_KeyMap[0])] = {0};
   LPCBYTE        keyState = (LPCBYTE)SDL_GetKeyboardState(NULL);
   int            i;
   DWORD          dwCurrentTime = SDL_GetTicks();

   for (i = 0; i < sizeof(g_KeyMap) / sizeof(g_KeyMap[0]); i++)
   {
      if (keyState[SDL_GetScancodeFromKey(g_KeyMap[i][0])])
      {
         if (dwCurrentTime > rgdwKeyLastTime[i])
         {
            PAL_KeyDown(g_KeyMap[i][1], (rgdwKeyLastTime[i] != 0));
            if (gConfig.fEnableKeyRepeat)
            {
               rgdwKeyLastTime[i] = dwCurrentTime + (rgdwKeyLastTime[i] == 0 ? 200 : 75);
            }
            else
            {
               rgdwKeyLastTime[i] = 0xFFFFFFFF;
            }
         }
      }
      else
      {
         if (rgdwKeyLastTime[i] != 0)
         {
            PAL_KeyUp(g_KeyMap[i][1]);
            rgdwKeyLastTime[i] = 0;
         }
      }
   }
}

static VOID
PAL_KeyboardEventFilter(
   const SDL_Event       *lpEvent
)
/*++
  Purpose:

    Handle keyboard events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
   if (lpEvent->type == SDL_KEYDOWN)
   {
      //
      // Pressed a key
      //
      if (lpEvent->key.keysym.mod & KMOD_ALT)
      {
         if (lpEvent->key.keysym.sym == SDLK_RETURN)
         {
            //
            // Pressed Alt+Enter (toggle fullscreen)...
            //
            VIDEO_ToggleFullscreen();
            return;
         }
         else if (lpEvent->key.keysym.sym == SDLK_F4)
         {
            //
            // Pressed Alt+F4 (Exit program)...
            //
            PAL_Shutdown(0);
         }
      }
      else if (lpEvent->key.keysym.sym == SDLK_p)
      {
         VIDEO_SaveScreenshot();
      }
#if PAL_HAS_GLSL
      else if (lpEvent->key.keysym.sym == SDLK_z)
      {
         Filter_StepParamSlot(1);
      }
      else if (lpEvent->key.keysym.sym == SDLK_x)
      {
         Filter_StepParamSlot(-1);
      }
      else if (lpEvent->key.keysym.sym == SDLK_COMMA)
      {
         Filter_StepCurrentParam(1);
      }
      else if (lpEvent->key.keysym.sym == SDLK_PERIOD)
      {
         Filter_StepCurrentParam(-1);
      }
#endif
   }
}

static VOID
PAL_MouseEventFilter(
   const SDL_Event *lpEvent
)
/*++
  Purpose:

    Handle mouse events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
}

static VOID
PAL_JoystickEventFilter(
   const SDL_Event       *lpEvent
)
/*++
  Purpose:

    Handle joystick events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
}



static VOID
PAL_TouchEventFilter(
   const SDL_Event *lpEvent
)
/*++
  Purpose:

    Handle touch events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    None.

--*/
{
}

static int SDLCALL
PAL_EventFilter(
   const SDL_Event       *lpEvent
)

/*++
  Purpose:

    SDL event filter function. A filter to process all events.

  Parameters:

    [IN]  lpEvent - pointer to the event.

  Return value:

    1 = the event will be added to the internal queue.
    0 = the event will be dropped from the queue.

--*/
{
   switch (lpEvent->type)
   {
   case SDL_WINDOWEVENT:
      if (lpEvent->window.event == SDL_WINDOWEVENT_RESIZED)
      {
         //
         // resized the window
         //
         VIDEO_Resize(lpEvent->window.data1, lpEvent->window.data2);
      }
      break;

   case SDL_APP_WILLENTERBACKGROUND:
      g_bRenderPaused = TRUE;
      break;

   case SDL_APP_DIDENTERFOREGROUND:
      g_bRenderPaused = FALSE;
      VIDEO_UpdateScreen(NULL);
      break;

   case SDL_QUIT:
      //
      // clicked on the close button of the window. Quit immediately.
      //
      PAL_Shutdown(0);
   }

   PAL_KeyboardEventFilter(lpEvent);
   PAL_MouseEventFilter(lpEvent);
   PAL_JoystickEventFilter(lpEvent);
   PAL_TouchEventFilter(lpEvent);

   //
   // All events are handled here; don't put anything to the internal queue
   //
   return 0;
}

VOID
PAL_ClearKeyState(
   VOID
)
/*++
  Purpose:

    Clear the record of pressed keys.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   g_InputState.dwKeyPress = 0;
}

VOID
PAL_InitInput(
   VOID
)
/*++
  Purpose:

    Initialize the input subsystem.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   memset((void *)&g_InputState, 0, sizeof(g_InputState));
   g_InputState.dir = kDirUnknown;
   g_InputState.prevdir = kDirUnknown;

   //
   // Check for joystick
   //

   input_init_filter();
}

VOID
PAL_ShutdownInput(
   VOID
)
/*++
  Purpose:

    Shutdown the input subsystem.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   input_shutdown_filter();
}

static int
PAL_PollEvent(
   SDL_Event *event
)
/*++
  Purpose:

    Poll and process one event.

  Parameters:

    [OUT] event - Events polled from SDL.

  Return value:

    Return value of PAL_PollEvent.

--*/
{
   SDL_Event evt;

   int ret = SDL_PollEvent(&evt);
   if (ret != 0 && !input_event_filter(&evt, &g_InputState))
   {
      PAL_EventFilter(&evt);
   }

   if (event != NULL)
   {
      *event = evt;
   }

   return ret;
}

VOID
PAL_ProcessEvent(
   VOID
)
/*++
  Purpose:

    Process all events.

  Parameters:

    None.

  Return value:

    None.

--*/
{
   while (PAL_PollEvent(NULL));

   PAL_UpdateKeyboardState();
}

VOID
PAL_RegisterInputFilter(
   void (*init_filter)(),
   int (*event_filter)(const SDL_Event *, volatile PALINPUTSTATE *),
   void (*shutdown_filter)()
)
/*++
  Purpose:

    Register caller-defined input event filter.

  Parameters:

    [IN] init_filter - Filter that will be called inside PAL_InitInput
	[IN] event_filter - Filter that will be called inside PAL_PollEvent, 
	                    return non-zero value from this filter disables
						further internal event processing.
	[IN] shutdown_filter - Filter that will be called inside PAL_ShutdownInput

	Passing NULL to either parameter means the caller does not provide such filter.

  Return value:

    None.

--*/
{
	if (init_filter)
		input_init_filter = init_filter;
	if (event_filter)
		input_event_filter = event_filter;
	if (shutdown_filter)
		input_shutdown_filter = shutdown_filter;
}
