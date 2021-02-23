//
// Created by jibbo on 2/22/21.
//

#ifndef MONOREPO_JSTRACESKI_ENTITY_H
#define MONOREPO_JSTRACESKI_ENTITY_H

#include <TinyMath.hpp>

/*!
 * \brief Structure to store entity data.
 *
 * Stores shape width/height and radius data.
 * Includes shape id and entity id data, as well as active state.
 */
struct Entity {
    Vector3D pos; /**<  position */
    Vector3D f_pos;  /**<  future position */
    Vector3D vel; /**<  velocity */

    int shapeId = 0; /**<  0 rect / 1 circle */
    int typeId = 0; /**<  0 paddle / 1 brick / 2 ball */
    int hits = 1; /**< counter for bricks, number of hits left */

    float radius = 0; /**< If the entity is a circle then this defines the radius, otherwise 0 */
    float width = 0; /**< If the entity is a rectangle then this defines the width, otherwise 0 */
    float height = 0; /**< If the entity is a rectangle then this defines the height, otherwise 0 */

    float drag = 0.95; /**<  air drag */

    bool reflects = false; /**<  does the object bounce when it collides */
    bool active = true; /**<  active state */

    Entity() = default;
};

#endif //MONOREPO_JSTRACESKI_ENTITY_H
