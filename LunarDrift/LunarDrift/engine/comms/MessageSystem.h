#pragma once

#include "IMessageListener.h"
#include <map>
#include <list>

/**
 * @brief Singletonized central processing system for messages and events
 *
 * This singleton encapsulates communication between entities in the program.
 * Events and messages are dispatched through this class for dispatching to
 * the relevant entities. For understanding, imagine if you will the idea of a
 * post office, wherein letters and newspapers are posted through; this class
 * is that post office. Consequently, its responsibilities include holding
 * information on "subscriptions" (who is subscribed to set events) and storing
 * names and addresses of "listeners" (for posting messages to), and processing
 * the dispatching of events and messages to those interested, respectively.
 *
 * @author Hayley Hatton
 * @date   10/03/2016
 * @see    IMessageListener
 * @see    Message
 * @ingroup Messages
 */
class MessageSystem
{
public:
    /**
     * @brief Singleton access
     */
	static MessageSystem& getInstance()
	{
		static MessageSystem instance;
		return instance;
	}

	/**
	 * @brief Register a class instance as a message listener
	 * @param name Name for posting messages to
	 * @param listener Pointer to class instance
	 */
	void registerListener(
		const std::string& name,
		IMessageListener* listener);

	/**
	 * @brief Unregister a message listener from being a listener
	 * @param name Name of the listener
	 */
	void unregisterListener(
		const std::string& name);

	/**
	 * @brief Unregister a class instance from being a listener
	 * @param listener Class instance pointer
	 */
	void unregisterListener(
		IMessageListener* listener);

	/**
	 * @brief Subscribe a class instance to receive events of a given name
	 * @param sevent Event name to subscribe to
	 * @param listener Pointer to the class instance
	 */
	void subscribe(
		const std::string& sevent,
		IMessageListener* listener);

	/**
	 * @brief Unsubscribe a class instance from receiving events of a given name
	 * @param sevent Event name to unsubscribe from
	 * @param listener Pointer to the class instance
	 */
	void unsubscribe(
		const std::string& sevent,
		IMessageListener* listener);

    /**
     * @brief Broadcast an event in the system to anyone interested
     * @param event Smart pointer to event
     */
	void broadcast(std::shared_ptr<Message> event);

	/**
	 * @brief Post a message to a registered listener
	 * @param to Name of the registered listener
	 * @param msg Smart pointer to the message
	 */
	void post(const std::string& to, std::shared_ptr<Message> msg);

	/**
	 * @brief Post a message to a listener
	 * @param listener Pointer to the listener
	 * @param msg Smart pointer to the message
	 */
	void post(IMessageListener* listener, std::shared_ptr<Message> msg);

private:
	// Singletonize
	MessageSystem() {};
	MessageSystem(MessageSystem const&) = delete;
	void operator=(MessageSystem const&) = delete;

	std::map<std::string, IMessageListener*> m_Listeners;
	std::map<std::string, std::list<IMessageListener*>> m_Subscribers;
};
