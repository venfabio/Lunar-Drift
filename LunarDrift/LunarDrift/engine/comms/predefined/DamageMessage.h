#pragma once

#include "../Message.h"
#include "../../containers/SceneObject.h"

/**
 * @brief Encapsulation of a damage dealt (or recovered) event
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Messages
 */
class DamageMessage : public Message
{
public:
	DamageMessage(
		SceneObject* target, 
		int damage, 
		SceneObject* source = nullptr
	);

	virtual ~DamageMessage();


	SceneObject* getTarget() { return m_Target; }
	
	SceneObject* getSource() { return m_Source; }

	int getDamage() const { return m_Damage; }

private:
	SceneObject* m_Target;
	SceneObject* m_Source;
	int m_Damage;
};
