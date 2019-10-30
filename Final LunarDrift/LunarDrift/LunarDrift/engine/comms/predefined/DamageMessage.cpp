#include "DamageMessage.h"


DamageMessage::DamageMessage(
	SceneObject* target,
	int damage,
	SceneObject* source
) : Message("Damage"),
	m_Target(target),
	m_Source(source),
	m_Damage(damage)
{
}

DamageMessage::~DamageMessage()
{
}
