#include "ScoreAddMessage.h"


ScoreAddMessage::ScoreAddMessage(bool isPlayer, int score)
	: Message("AddScore"),
	  m_Score(score), 
	  m_IsPlayer(isPlayer)
{
}

ScoreAddMessage::~ScoreAddMessage()
{

}
