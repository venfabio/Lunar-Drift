#include "MessageSystem.h"

void MessageSystem::registerListener(
	const std::string& name,
	IMessageListener* listener)
{
	m_Listeners.emplace(name, listener);
}

void MessageSystem::unregisterListener(const std::string& name)
{
	m_Listeners.erase(name);
}

void MessageSystem::unregisterListener(IMessageListener* listener)
{
	for(auto itr = m_Listeners.begin(); itr != m_Listeners.end(); )
    {
        if(itr->second == listener)
            m_Listeners.erase(itr++);
        else
            ++itr;
    }
}

void MessageSystem::subscribe(
	const std::string& sevent,
	IMessageListener* listener)
{
	for (IMessageListener* listnr : m_Subscribers[sevent])
	{
		if (listnr == listener)
			return;
	}

	m_Subscribers[sevent].push_back(listener);
}

void MessageSystem::unsubscribe(
	const std::string& sevent,
	IMessageListener* listener)
{
	m_Subscribers[sevent].remove(listener);
}

void MessageSystem::broadcast(std::shared_ptr<Message> event)
{
	if (m_Subscribers.find(event->getType()) == m_Subscribers.end())
		return;

	std::list<IMessageListener*>& subs = m_Subscribers.at(event->getType());

	for (auto sub : subs)
		static_cast<IMessageListener*>(sub)->onMessage(event);
}

void MessageSystem::post(const std::string& to, std::shared_ptr<Message> msg)
{
	m_Listeners.at(to)->onMessage(msg);
}

void MessageSystem::post(IMessageListener* target, std::shared_ptr<Message> msg)
{
	/*auto eventList = m_Subscribers.find(msg->getType());
	if (eventList != m_Subscribers.end())
	{
		for (IMessageListener* listener : eventList->second)
		{
			if (listener == target)
				target->onMessage(msg);
		}
	}*/
}
