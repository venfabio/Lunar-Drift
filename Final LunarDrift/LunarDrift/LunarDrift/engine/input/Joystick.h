#pragma once

#include <glm/glm.hpp>

/**
 * @brief Joystick input representation object
 *
 * This class manages abstract joystick state whilst providing a mapping
 * between hardware input state and abstract analogue-typed input classes.
 *
 * @author Hayley Hatton
 * @date   13/03/2016
 * @see    InputMap
 * @ingroup Inputs
 */
class Joystick
{
public:
    /** Default constructor */
    Joystick();
    /** Default destructor */
    virtual ~Joystick();

protected:

private:
    glm::vec2 m_Position;
};
