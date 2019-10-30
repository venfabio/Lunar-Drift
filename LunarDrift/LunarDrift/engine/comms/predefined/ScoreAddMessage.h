#pragma once

#include "../Message.h"

/**
 * @brief Encapsulation of a request to modify a player's score
 *
 * @author Hayley Hatton
 * @date   20/05/2016
 * @ingroup Messages
 */
class ScoreAddMessage : public Message
{
public:
	/** Default constructor */
	ScoreAddMessage(bool isPlayer, int score);

	/** Default destructor */
	~ScoreAddMessage();

	int getScoreAdd() const {  return m_Score; }

	bool getPlayer() const {  return m_IsPlayer; }
	

protected:

private:
	int m_Score;
	bool m_IsPlayer;
};