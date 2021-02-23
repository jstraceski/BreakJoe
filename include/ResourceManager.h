//
// Created by jibbo on 2/22/21.
//

#ifndef MONOREPO_JSTRACESKI_RESOURCEMANAGER_H
#define MONOREPO_JSTRACESKI_RESOURCEMANAGER_H

#include <map>
#include <LOpenGL.h>
#include <vector>
#include <cstring>
#include <Entity.h>
#include <Clip.h>
#include "Game.h"

/*!
 * \brief Structure to store character texture data.
 *
 * Stores the texture id, width, height, and position of the character glyph.
 */
struct Glyph {
    unsigned int texId;     /**<  texture id of the glyph */
    unsigned int width;     /**<  character width */
    unsigned int height;    /**<  character height */
    int x;                  /**<  offset to the bottom left of the glyph x */
    int y;                  /**<  offset to the bottom left of the glyph y */
    long advance;            /**<  advance to the next glyph */
};

/*!
 * \brief Singleton to represent all game data.
 *
 * Stores Character Textures, Sound Clips, Key-States, Message Text, Level Lookup.
 */
class ResourceManager {
private:
    ResourceManager();                      /**<  Private Constructor */
    ResourceManager(ResourceManager const&); /**<  Avoid copy constructor */
    void operator=(ResourceManager const&); /**<  Don't allow assignment. */

    std::map<char, Glyph> glyphLookup;                  /**<  character glyph lookup */
    std::map<std::string, std::string> messageLookup;   /**<  display message lookup table */
    std::map<std::string, Clip*> soundLookup;           /**<  sound clip lookup table */
    std::vector<std::string> levels;                    /**<  level save data lookup table */
    std::map<std::string, std::string> menuLookup;      /**<  lookup table for menu options to language files */
    std::map<SDL_Keycode, bool> keyState;               /**<  table to store persistent key states */

public:

    std::vector<Entity *> entities;     /**<  entity display and physics list */

    std::vector<std::string> menuOptions;   /**<  language menu option lists */
    int menuIndex = 0;                      /**<  menu selection index */

    Entity* player; /**<  player entity pointer */
    Entity* ball;   /**<  ball entity pointer */

    int levelId = 0;        /**<  current level id */
    int playerLives = 3;    /**<  number of player lives */
    int score = 0;          /**<  current score */

    float pauseTimer = 0;           /**<  pause timer */
    float PAUSE_DELAY = 3;          /**<  default pause delay */
    std::string pauseText = "";     /**<  pause text string */

    bool end = false;           /**<  game over */
    bool menu = false;          /**<  is the game in a menu state */
    bool ballCaptured = true;   /**<  is the ball captured on the paddle */

    Vector3D shootVector = Vector3D(0, 2, 0); /**<  initial velocity when shooting the ball from the paddle */

    /*!
     * \brief Obtain the static instance.
     * @return static instance of Resource Manager
     */
    static ResourceManager *getInstance();

    /*!
     * \brief Load a sound file from a path.
     *
     * Stores the loaded sound clip to be referenced by the key string.
     *
     * @param path system path to sound file
     * @param key lookup string
     * @return pointer to the loaded sound clip
     */
    Clip* loadSound(const char * path, const std::string& key);

    /*!
     * \brief Queue the sound clip to play.
     *
     * Key is a reference to the string used when loadSound is called.
     *
     * @param key lookup string
     */
    void playSound(const std::string& key);


    /*!
     * \brief Load font graphics from path.
     *
     * @param ft Freetype library instance
     * @param path system path to the ttf file
     * @return 0 if the font fails to load, 1 otherwise
     */
    int loadFont(FT_Library ft, const char * path);

    /*!
     * \brief Load language strings from a text file.
     *
     * Strings are separated by newlines and are in order.
     * @param path system path to the language text file
     */
    void loadLanguage(std::string path);

    /*!
     * \brief Obtain display text from a key.
     *
     * This is used so that text can be mapped by a key and loaded by a language file.
     * @param key reference key
     * @return referenced text
     */
    std::string getText(const std::string& key);


    /*!
     * \brief Update and store key state.
     *
     * Used to store the current state of a key irrespective of key-events.
     * @param key keyboard keycode
     * @param state true if the key is down, false otherwise
     */
    void updateKey(SDL_Keycode key, bool state);

    /*!
     * \brief Get the state of a key.
     * @param key keyboard keycode
     * @return true if the key is down, false is up
     */
    bool getKey(SDL_Keycode key);


    /*!
     * \brief Clear all bricks from the entity vector, leaves players and balls.
     */
    void clearLevel();

    /*!
     * \brief Load level data from a file path.
     *
     * Level data is stored in lines where numbers represent how many hits a brick can take. i.e.
     *  0123210 represents a row of ascending and descending brick values surround by empty space.
     *
     * @param path system path to the level file
     */
    void loadLevel(std::string path);

    /*!
     * \brief Update the current level state.
     * @param win true if the level was won, false otherwise
     */
    void levelUpdate(bool win);


    /*!
     * \brief Increment a menu selection index by i
     * @param i index offset
     */
    void menuIncrement(int i);

    /*!
     * \brief Function called when enter is pressed and a menu is open.
     *
     * In this case it selects a language file to load.
     */
    void menuFunction();


    /*!
     * \brief Draw text to the screen.
     *
     * Draws the text at the given position, size scale, and alignment.
     * The alignment is where the text is drawn from.
     *
     * @param text render string
     * @param pos text position
     * @param scale text size
     * @param alignment left/center/right -> 0/1/2
     */
    void drawText(const std::string& text, const Vector3D &pos, float scale, int alignment);

    /*!
     * \brief Draw entity to the screen.
     * @param e entity reference
     */
    static void drawEntity(const Entity *e);


    /*!
     * \brief Setup default value.
     * @return 0 if the setup failed, 1 otherwise
     */
    int startUp();

    /*!
     * \brief Dereference all pointers and clean up data.
     * @return 0 if the data is deleted, 1 otherwise
     */
    int shutDown();

};

#endif //MONOREPO_JSTRACESKI_RESOURCEMANAGER_H
