#pragma once

#include "../Message.h"

/**
 * @brief Encapsulation of a request to fire a missile
 *
 * @author Hayley Hatton
 * @date   17/05/2016
 * @ingroup Messages
 */
class FireMissileMessage: public Message
{
public:
    /** Default constructor */
	FireMissileMessage(bool shooterIsPlayer1);

    /** Default destructor */
	virtual ~FireMissileMessage();


	bool isShooterPlayer1() const { return m_ShooterIsPlayer1; }

private:
	bool m_ShooterIsPlayer1;
};
