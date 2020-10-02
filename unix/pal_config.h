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
// unix/pal_config.h: Linux & Unix specific header.
//           @Author: Lou Yihua <louyihua@21cn.com>, 2017.
//

#ifndef PAL_CONFIG_H
# define PAL_CONFIG_H


# if SDL_MAJOR_VERSION == 1 && SDL_MINOR_VERSION <= 2
#  define PAL_HAS_SDLCD         1
# endif

# define PAL_PREFIX            "./"
# define PAL_SAVE_PREFIX       "./"

# if APPIMAGE
#  undef PAL_PREFIX
#  define PAL_PREFIX            PAL_va(PAL_MAX_GLOBAL_BUFFERS-1,"%s%s", SDL_getenv("HOME"),"/.config")
# endif

# define PAL_DEFAULT_WINDOW_WIDTH   640
# define PAL_DEFAULT_WINDOW_HEIGHT  400
# define PAL_DEFAULT_FULLSCREEN_HEIGHT 480

#  define PAL_VIDEO_INIT_FLAGS  (SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | (gConfig.fFullScreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0))

# define PAL_SDL_INIT_FLAGS	(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE | SDL_INIT_JOYSTICK)

# define PAL_PLATFORM         NULL
# define PAL_CREDIT           NULL
# define PAL_PORTYEAR         NULL

#include <sys/time.h>

#endif
