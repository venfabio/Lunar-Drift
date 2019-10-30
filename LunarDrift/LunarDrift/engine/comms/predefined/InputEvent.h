#pragma once

#include "../Message.h"

/**
 * @brief Encapsulation of any input-related event
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Messages
 */
class InputEvent : public Message
{
public:
    /** Default constructor */
    InputEvent(const std::string& type);
    /** Default destructor */
    virtual ~InputEvent();

protected:

private:
};
