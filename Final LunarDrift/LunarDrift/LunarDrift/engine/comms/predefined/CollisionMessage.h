#pragma once

#include "../Message.h"
#include "../../containers/SceneObject.h"

/**
 * @brief Encapsulation of the participants of a collision event
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Messages
 */
class CollisionMessage : public Message
{
public:
    /**
     * @brief Constructor
     * @param partner Partner of a collision
     * @param collisionPointID Collision sphere ID of collidee
     */
	CollisionMessage(SceneObject* partner, unsigned int collisionPointID);

	virtual ~CollisionMessage();


	SceneObject* getPartner() { return m_Partner; }

    unsigned int getCollisionPointID() const { return m_CollisionPointID; }

private:
	SceneObject* m_Partner;
    unsigned int m_CollisionPointID;
};
