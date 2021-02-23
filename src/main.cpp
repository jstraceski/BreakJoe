//
// Created by jibbo on 2/22/21.
//

#include <Game.h>

/*!
 * Main entry point
 *
 * Initializes A Game object and starts off the main loop.
 * @param argc
 * @param args
 * @return
 */
int main(int argc, char* args[])
{
    Game g;
    if (g.init()) {
        g.run();
    }
    return 0;
}
