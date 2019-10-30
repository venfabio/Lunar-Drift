#pragma once

#include "../Message.h"
#include "../../containers/SceneObject.h"

/**
 * @brief Encapsulation of an event pertaining an object that's died
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Messages
 */
class DeathMessage : public Message
{
public:
	DeathMessage(SceneObject* doomed, SceneObject* killer = nullptr);

	~DeathMessage();


	SceneObject* getDoomed() const { return m_Doomed; }

	SceneObject* getKiller() const { return m_Killer; }
	
private:
	SceneObject* m_Doomed;
	SceneObject* m_Killer;
};
