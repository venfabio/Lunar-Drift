#pragma once

#include <string>

/**
 * @defgroup Messages
 * @brief List of messages and events that can be broadcasted and subscribed for in the engine
 */

/**
 * @brief Base class for messages
 *
 * Messages are packets of data sent between entities in the program.
 * Events operate in a broadcast fashion, messages operate in a unicast fashion
 * with a sender and a receiver at the point of handling.
 *
 * @author Hayley Hatton
 * @date   10/03/2016
 * @see    IMessageListener
 * @see    MessageSystem
 * @ingroup Messages
 */
class Message
{
public:
    /**
     * @brief Default constructor
     * @param type Type identifier string
     */
    Message(const std::string& type);

    /** @brief Default destructor */
    virtual ~Message();


    /**
     * @brief Access the type identifier of the derived message subclass
     * @return Type identifier string
     */
    const std::string& getType() const { return m_Type; }

protected:

private:
    const std::string m_Type;   //!< Type identifier of the derived class
};
