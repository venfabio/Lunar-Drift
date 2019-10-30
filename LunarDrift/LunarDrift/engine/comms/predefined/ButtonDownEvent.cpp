#include "ButtonDownEvent.h"

ButtonDownEvent::ButtonDownEvent(const std::string& inputName)
    : InputEvent("ButtonDown"),
      m_InputName(inputName)
{
    //ctor
}

ButtonDownEvent::~ButtonDownEvent()
{
    //dtor
}
