#pragma once

#include "Keyboard.h"

/**
 * @defgroup Inputs
 * @brief Classes for handling and accessing input peripheral states and events
 */

/**
 * @brief Singletonized encapsulation of inputs and how they map to peripherals
 *
 * See the individual peripherals for more information on how they map
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @see    Keyboard
 * @ingroup Inputs
 */
class InputMap
{
public:
	static InputMap& getInstance()
	{
		static InputMap instance;
		return instance;
	}

    Keyboard* getKeyboard() { return &m_Keyboard; }


protected:

private:
    Keyboard m_Keyboard;
    //std::vector<Joystick> m_Joysticks;

	// Singletonize
	InputMap() {};
	InputMap(InputMap const&) = delete;
	void operator=(InputMap const&) = delete;
};
