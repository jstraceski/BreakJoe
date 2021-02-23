## Add any additional notes here

A and D to move left and right

Enter to select a language

Space to shoot

Q to quit

Because SDL_ttf does not support the direct manipulation of font character textures 
(in order to use it with opengl you have to first render them to a texture and then render them to the screen) 
I opted to use a open source ttf library call freetype.
Im not sure if the binaries I built for the project and have included in the lib folder will work on every linux installation. 
Because I only had a few days to do this assignment I couldn't find an easy alternative. 
If the assignment fails to build because of this I will use a different method in the future.

The library I used is called the freetype library and if you want to try and use the libraries it is available here https://gitlab.freedesktop.org/freetype/freetype.

On a more interesting note this is kind of a historically significant library as it was one of the first projects created to load and render true type fonts.

## Game Publicity

**https://endlessarc.net/game_development/index.html**

## Compilation (and execution) Instructions

```python

sudo apt install libsdl2-dev libsdl2-2.0-0
sudo apt install libsdl2-image-dev libsdl2-image-2.0-0
sudo apt install libfreetype6-dev libsdl2-ttf-dev libsdl2-ttf-2.0-0

python build.py
./bin/breakjoe
```
There shouldn't be any issues but see the first bullet point in case of any snags.

## Project Hieararchy

### ./Game Directory Organization

- Docs 
    - Source Code Documentation
- Assets
    - Art assets
- src
    - source code
- include
    - header files
- lib
    - libraries (.so, .dll, .a, .dylib files)
- bin
    - binary of the game built on Ubuntu20.04 x64 
- GameBuild (Optional)
    - full build of the game (built on Ubuntu20.04 x64)


I have used:
DevIL image library http://openil.sourceforge.net/
freetype ttf library https://www.freetype.org/history.html

Sounds are From:
https://freesound.org/people/Nick_Simon-Adams/sounds/515091/
https://freesound.org/people/Eponn/sounds/528865/
