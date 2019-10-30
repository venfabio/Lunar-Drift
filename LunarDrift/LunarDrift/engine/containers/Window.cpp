#include "Window.h"

Window::Window() : m_Program(nullptr)
{
    //ctor
}

Window::~Window()
{
    //dtor
}


//ExitStatus Window::run(Program* program)
//{
//    m_Program = program;
//    return ExitStatus::Success;
//}

std::string Window::getGPUName() const
{
	std::string vendor(
		reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	std::string renderer(
		reinterpret_cast<const char*>(glGetString(GL_RENDERER)));

	std::string output;
	output += std::string(vendor.cbegin(), vendor.cend());
	output += ": ";
	output += std::string(renderer.cbegin(), renderer.cend());

	return output;
}
