#include "SceneActivateMessage.h"

SceneActivateMessage::SceneActivateMessage(const std::string& sceneName)
	: Message("SceneActivate"),
	  m_SceneName(sceneName)
{
    //ctor
}

SceneActivateMessage::~SceneActivateMessage()
{
    //dtor
}
