//
// Created by jibbo on 2/22/21.
//


#ifndef MONOREPO_JSTRACESKI_CLIP_H
#define MONOREPO_JSTRACESKI_CLIP_H

#include <LOpenGL.h>
#include <vector>

/*!
 * \brief Struct to hold music data
 *
 * Includes looping and tracking of music position.
 */
struct Clip {
public:
    Uint32 clipLen;         /**<  clip length */
    Uint32 trackPos = 0;    /**<  track position */

    Uint8 *data;            /**<  audio data */
    SDL_AudioSpec spec;     /**<  audio specs */

    bool loop = false;      /**<  flag to set the clip looping */
    bool active = false;    /**<  is the clip playing or not */
    static std::vector<Clip * > sounds; /**< static list of all sounds */

    /*!
     * \brief Callback passed into the SDL Audio library.
     *
     * Loops through all loaded sounds and mixes them together if they are active.
     *
     * @param userdata not used
     * @param stream audio buffer
     * @param len length of the buffer
     */
    static void callback(void *userdata, Uint8 *stream, int len) {
        memset(stream, 0, len);

        for (Clip* s : sounds) {
            if (s->active) {
                if (s->trackPos + len >= s->clipLen) {
                    Uint32 last = s->clipLen - s->trackPos;
                    Uint32 extra = len - last;
                    SDL_MixAudio(stream, &s->data[s->trackPos], last, SDL_MIX_MAXVOLUME);
                    if (s->loop) {
                        SDL_MixAudio(&stream[last], &s->data[0], extra, SDL_MIX_MAXVOLUME);
                        s->trackPos = extra;
                    } else {
                        s->active = false;
                    }
                } else {
                    SDL_MixAudio(stream, &s->data[s->trackPos], len, SDL_MIX_MAXVOLUME);
                    s->trackPos += len;
                }
            }
        }
    }

    /*!
     * Deconstructor
     *
     * Deletes the buffer data.
     */
    ~Clip() {
        delete data;
    }
};

#endif //MONOREPO_JSTRACESKI_CLIP_H
