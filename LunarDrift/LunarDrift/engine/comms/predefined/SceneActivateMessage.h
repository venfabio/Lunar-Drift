#pragma once

#include "../Message.h"
#include <string>

/**
 * @brief Encapsulation of a transition to a given scene
 *
 * @author Hayley Hatton
 * @date   07/03/2016
 * @ingroup Messages
 */
class SceneActivateMessage: public Message
{
public:
    /** Default constructor */
	SceneActivateMessage(const std::string& sceneName);

    /** Default destructor */
	virtual ~SceneActivateMessage();


	const std::string& getSceneName() const { return m_SceneName; }

protected:

private:
    const std::string m_SceneName;  //!< Mapped name of the scene to activate
};
