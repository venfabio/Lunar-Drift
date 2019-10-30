#pragma once

#include "InputEvent.h"

/**
 * @brief Encapsulation of a button-down event
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Messages
 */
class ButtonDownEvent : public InputEvent
{
public:
    /** Default constructor */
    ButtonDownEvent(const std::string& inputName);

    /** Default destructor */
    virtual ~ButtonDownEvent();


    const std::string& getInputName() const { return m_InputName; }

protected:

private:
    const std::string m_InputName;  //!< Mapped name of the associated input btn

};
