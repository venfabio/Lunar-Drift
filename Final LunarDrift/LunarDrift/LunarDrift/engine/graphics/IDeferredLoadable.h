#pragma once

#include "Context.h"

/**
 * @brief Allows a class to be loadable after the constructor has been called
 *
 * For most classes, the desired implication is to be completely constructed by
 * the constructor, including all subclasses and variable instances. For 
 * graphical objects, the need for a Context binds the object construction
 * to the rendering thread. This is where this interface comes in: for objects
 * who can be created without the guarantee of the rendering thread (e.g.
 * in response to Messages), they need the Context provided later. This
 * interface permits such objects to do their loading separately.
 *
 * @author Hayley Hatton
 * @date   19/03/2016
 * @ingroup Graphics
 */
class IDeferredLoadable
{
public:
    /**
     * @brief Called to load the object
     * @param context Graphics context
     */
    virtual void load(Context* context) = 0;

    /**
    * @brief Called to destroy the object
    * @param context Graphics context
    */
    virtual void unload(Context* context) = 0;
};
