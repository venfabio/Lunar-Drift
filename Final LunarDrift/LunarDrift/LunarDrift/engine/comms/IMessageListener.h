#pragma once

#include "Message.h"
#include <memory>

/**
 * @brief Interface for handling messages sent from other entities
 *
 * Messages are packets of data sent between entities in the program.
 * Events operate in a broadcast fashion, messages operate in a unicast fashion
 * with a sender and a receiver at the point of handling.
 *
 * @author Hayley Hatton
 * @date   10/03/2016
 * @see    Message
 * @see    MessageSystem
 */
class IMessageListener
{
public:
    virtual ~IMessageListener() {};

    /**
     * @brief Called when a message is sent to this entity
     * @param msg Smart pointer to the received message object
     */
    virtual void onMessage(std::shared_ptr<Message> msg) = 0;
};
