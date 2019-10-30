#include "ButtonUpEvent.h"

ButtonUpEvent::ButtonUpEvent(const std::string& inputName)
    : InputEvent("ButtonUp"),
      m_InputName(inputName)
{
    //ctor
}

ButtonUpEvent::~ButtonUpEvent()
{
    //dtor
}
