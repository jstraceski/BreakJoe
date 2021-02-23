//
// Created by jibbo on 2/22/21.
//

#ifndef MONOREPO_JSTRACESKI_LOPENGL_H
#define MONOREPO_JSTRACESKI_LOPENGL_H

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_image.h>
    #include <SDL2/SDL_ttf.h>
    #include <IL/il.h>
    #include <IL/ilu.h>
    #include <IL/ilut.h>
#else // This works for Mac
    #include <SDL.h>
    #include <SDL_image.h>
    #include <SDL_ttf.h>
    #include <IL/il.h>
    #include <IL/ilu.h>
    #include <IL/ilut.h>
#endif


#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <GL/gl.h>
#include <GL/glu.h>




#endif //MONOREPO_JSTRACESKI_LOPENGL_H
