#include "DeathMessage.h"


DeathMessage::DeathMessage(SceneObject* doomed, SceneObject* killer)
	: Message("Death"),
	  m_Doomed(doomed),
	  m_Killer(killer)
{
}

DeathMessage::~DeathMessage()
{
}
