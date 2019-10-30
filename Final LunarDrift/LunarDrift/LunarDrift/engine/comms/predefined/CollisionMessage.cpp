#include "CollisionMessage.h"


CollisionMessage::CollisionMessage(
    SceneObject* partner,
    unsigned int collisionPointID)
	: Message("Collision"),
	  m_Partner(partner),
      m_CollisionPointID(collisionPointID)
{
}

CollisionMessage::~CollisionMessage()
{
}
