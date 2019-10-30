#include "Keyboard.h"
#include "../comms/MessageSystem.h"
#include "../comms/predefined/ButtonDownEvent.h"
#include "../comms/predefined/ButtonUpEvent.h"
#define __STDC_CONSTANT_MACROS
#include <cstdint>
#include <cassert>

Keyboard::Keyboard()
	: m_Recorder(nullptr)
{
    m_KeyStates.resize(UINT16_MAX);
}

Keyboard::~Keyboard()
{
    //dtor
}



Key Keyboard::getKeyAssociatedWithMap(const std::string& inputName) const
{
	assert(inputName.length() > 0);

	for (auto itr = m_KeyInputMap.begin(); itr != m_KeyInputMap.end(); ++itr)
	{
		if (inputName.compare(itr->second) == 0)
			return itr->first;
	}
	throw std::out_of_range("Requested input map name key not found");
}

void Keyboard::mapKeyToInput(Key key, const std::string& inputName)
{
    m_KeyInputMap.emplace(key, inputName);
}

void Keyboard::unmapKeyFromInputs(Key key)
{
    m_KeyInputMap.erase(key);
}

bool Keyboard::isKeyDown(const std::string& id)
{
    assert(id.length() > 0);

    for(auto itr = m_KeyInputMap.begin(); itr != m_KeyInputMap.end(); ++itr)
    {
        if(id.compare(itr->second) == 0)
            return m_KeyStates[itr->first];
    }
    throw std::out_of_range("Requested input map name key not found");
}

void Keyboard::setKeyDown(Key key)
{
	if (m_KeyStates[key] == true)
		return;

	if (m_Recorder != nullptr)
		m_Recorder->onKeyDown(key);

    m_KeyStates[key] = true;

    if(m_KeyInputMap.find(key) == m_KeyInputMap.end())
        return;
    MessageSystem::getInstance().broadcast(
        std::make_shared<ButtonDownEvent>(m_KeyInputMap[key]));
}

void Keyboard::setKeyUp(Key key)
{
	if (m_KeyStates[key] == false)
		return;

	if (m_Recorder != nullptr)
		m_Recorder->onKeyUp(key);

    m_KeyStates[key] = false;

    if(m_KeyInputMap.find(key) == m_KeyInputMap.end())
        return;
    MessageSystem::getInstance().broadcast(
        std::make_shared<ButtonUpEvent>(m_KeyInputMap[key]));
}