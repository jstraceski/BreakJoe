//
// Created by jibbo on 2/1/21.
//

#ifndef MONOREPO_JSTRACESKI_LTIMER_H
#define MONOREPO_JSTRACESKI_LTIMER_H

#if defined(LINUX) || defined(MINGW)
    #include <SDL2/SDL.h>
#else // This works for Mac
    #include <SDL.h>
#endif



/*!
 * \brief Lazy-foo Timer wrapper.
 *
 * Stores time data and makes managing time easier.
 */
class LTimer
{
public:
    //Initializes variables
    LTimer();
    
    //The various clock actions
    /*!
     * Start the timer.
     */
    void start();

    /*!
     * Stop the timer.
     */
    void stop();

    /*!
     * Pause the timer.
     */
    void pause();

    /*!
     * Resume the timer.
     */
    void unpause();
    
    //Gets the timer's time
    /*!
     * Get the total time in ticks since the timer was started.
     * @return time in milliseconds
     */
    Uint32 getTicks();

    //Checks the status of the timer
    /*!
     * Has the timer been started.
     * @return true if it has, false otherwise
     */
    bool isStarted();

    /*!
    * Has the timer been paused.
    * @return true if it has, false otherwise
    */
    bool isPaused();

private:
    //The clock time when the timer started
    Uint32 mStartTicks;
    
    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;
    
    //The timer status
    bool mPaused;
    bool mStarted;
};


#endif //MONOREPO_JSTRACESKI_LTIMER_H
