#include "FireMissileMessage.h"

FireMissileMessage::FireMissileMessage(bool shooterIsPlayer1)
	: Message("FireMissile"),
	  m_ShooterIsPlayer1(shooterIsPlayer1)
{
    //ctor
}

FireMissileMessage::~FireMissileMessage()
{
    //dtor
}
