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
python build.py
./bin/breakjoe
```
There shouldn't be any issues but see the first bullet point in case of any snags.

## Project Hieararchy

In the future, other engineers may take on our project, so we have to keep it organized given the following requirements below. Forming some good organization habits now will help us later on when our project grows as well. This is the required project hierarchy you should have.

### ./Game Directory Organization

- Docs 
    - Source Code Documentation
- Assets
    - Art assets (With the Sub directories music, sound, images, and anything else)
- src
    - source code(.cpp files) The make file or any build scripts that automate the building of your project should reside here.
- include
    - header files(.h and .hpp files)
- lib
    - libraries (.so, .dll, .a, .dylib files). Note this is a good place to put SDL
- bin
    - This is the directory where your built executable(.exe for windows, .app for Mac, or a.out for Linux) and any additional generated files are put after each build.
- GameBuild (Optional)
    - You may optionally put a .zip to you final deliverable. One should be able to copy and paste this directory, and only this directory onto another machine and be able to run the game. This is optional because for this course we will be building your projects from source. However, in the game industry it is useful to always have a build of a game ready for testers, thus a game project hieararchy would likely have this directory in a repo or other storage medium.
- ThirdParty
    - Code that you have not written if any.

I have used:
DevIL image library http://openil.sourceforge.net/
freetype ttf library https://www.freetype.org/history.html

Sounds are From:
https://freesound.org/people/Nick_Simon-Adams/sounds/515091/
https://freesound.org/people/Eponn/sounds/528865/

1. src and include should only contain ".cpp" or ".hpp" files. Why? It makes it very fast to do a backup of your game project as one example. Secondly, binary files that are generated often clutter up directories. I should not see any binaries in your repository, you may use a '.gitignore' file to help prevent this automatically. 
