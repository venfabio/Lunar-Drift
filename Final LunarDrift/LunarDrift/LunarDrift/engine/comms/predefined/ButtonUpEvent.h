#pragma once

#include "InputEvent.h"

/**
 * @brief Encapsulation of a button-up event
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Messages
 */
class ButtonUpEvent : public InputEvent
{
public:
    /** Default constructor */
    ButtonUpEvent(const std::string& inputName);

    /** Default destructor */
    virtual ~ButtonUpEvent();


    const std::string& getInputName() const { return m_InputName; }

protected:

private:
    const std::string m_InputName;  //!< Mapped name of the associated input btn
};
