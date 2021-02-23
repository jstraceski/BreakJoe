//
// Created by jibbo on 2/22/21.
//

#ifndef MONOREPO_JSTRACESKI_GAME_H
#define MONOREPO_JSTRACESKI_GAME_H

#include <LOpenGL.h>
#include <LTimer.h>
#include <ResourceManager.h>

/**
 * \brief Window and Game Container.
 *
 * Initializes OpenGL/DevIL/Freetype library data and controls most game logic.
 * Gathers key input, parses game states, updates resulting states, and renders the result ot the screen.
 */
struct Game {
private:
    /*!
     * Render Entity States and Menu Objects stored in the ResourceManager.
     */
    void render();

    /*!
     * \brief Update ResourceManager key data and key state.
     *
     * Called every key event.
     * @param key keycode
     * @param down key state
     */
    void handleKey(SDL_Keycode key, bool down);

    /*!
     * Parse input states, called every tick.
     */
    void input();

    /*!
     * Initialize OpenGL data.
     * @return false if the init fails, true otherwise
     */
    bool initGL();

    /*!
     * \brief Load media data.
     * Loads tff data, image data, game data, and initializes the ResourceManager.
     * @return  false if the loading media fails, true otherwise
     */
    bool loadMedia();

    /*!
     * \brief Preform Physics and State Updates.
     */
    void update();

    /*!
     * \brief Syncs the rendering to cap the frame rate.
     */
    void sync();

    /*!
     * \brief Close and delete all data.
     * Calls the ResourceManager deconstruct to.
     */
    void close();

    bool quit = false; /**< quit flag */

    GLuint gProgramID = 0; /**< The window we'll be rendering to */
    SDL_Window* gWindow = NULL; /**< SDL window pointer */

    SDL_GLContext gContext; /**< SDL OpenGL context */

    LTimer fpsTimer; /**< Fps capping timer */

    int beforeTick; /**< last time from the timer */
    int afterTick; /**< current time from the timer */

    const int SCREEN_FPS = 60; /**< Capped FPS */
    const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS; /**< ms ticks per frame */

public:
    static int SCREEN_WIDTH;    /**<  Screen Width */
    static int SCREEN_HEIGHT;   /**<  Screen Height */

    static int BRICK_TOP_OFFSET;    /**<  offset between the top of the bricks and the top of the screen. */
    static int BRICK_HEIGHT;        /**<  height of the brick on the screen */
    static int BRICK_SPACING;       /**<  space between bricks */

    static float PADDLE_SPEED;      /**<  paddle speed added to the velocity of the paddle every tick */
    static float MAX_SPEED;         /**<  ball and paddle max speed */
    Game();

    /*!
     * Initialize and load all data.
     * @return false if the init fails, true otherwise
     */
    bool init();

    /*!
     * Start the game loop.
     * Main entrance to the game, obtains key inputs, parses states, and renders the changes.
     */
    void run();
};


#endif //MONOREPO_JSTRACESKI_GAME_H
