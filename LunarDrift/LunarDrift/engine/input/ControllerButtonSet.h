#pragma once

/**
 * @brief Represents a controller as a set of buttons
 *
 * This class never found use. Proxying XInput into the keyboard
 * found more use to be honest.
 *
 * @author Hayley Hatton
 * @date   27/05/2016
 * @see    InputMap
 * @see    Keyboard
 * @ingroup Inputs
 */
class ControllerButtonSet
{
public:
    /** Default constructor */
    ControllerButtonSet();
    /** Default destructor */
    virtual ~ControllerButtonSet();

protected:

private:
};
