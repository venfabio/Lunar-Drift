#include "game/LunarDriftProgram.h"
#include "engine/containers/WindowPC.h"
#include "engine/exceptions/Exceptions.h"
#include <iostream>


int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow) 
{
	try
	{
		WindowPC environment(LunarDriftProgram::ms_Name, hInstance, lpCmdLine, nCmdShow, 800, 600);
		std::unique_ptr<LunarDriftProgram> program = std::make_unique<LunarDriftProgram>(environment.getContext());
		ExitStatus status = environment.run(std::move(program));
		return static_cast<int>(status);
	}
	catch (ShaderException& ex)
	{
		std::string msg(ex.getFile() + " (" + ex.getComponent() + " " +
			ex.getStage() + " failed)\r\n" + ex.getErrorLog());

		MessageBox(nullptr, msg.c_str(), "Shader Loading Error", MB_ICONERROR);
	}
	catch (OpenGLException& ex)
	{
		std::string msg(ex.getReport() + " (" +
			ex.glErrorCodeToString(ex.getGLErrorCode()) + ")");

		MessageBox(nullptr, msg.c_str(), "OpenGL Error", MB_ICONERROR);
	}
	catch (ConfigurationException& ex)
	{
		std::string msg("A configuration setting is invalid!\r\n" +
			ex.getSetting() + " " + ex.getError());

		MessageBox(nullptr, msg.c_str(), "Configuration Error", MB_ICONERROR);
	}
	catch (FileIOException& ex)
	{
		std::string msg(ex.getFile() + ": " + ex.getError());

		MessageBox(nullptr, msg.c_str(), "File I/O Error", MB_ICONERROR);
	}
	catch (UnsupportedFeatureException& ex)
	{
		std::string msg("A feature is unsupported!\r\n" +
			ex.getFeature() + ": " + ex.getReason());

		MessageBox(nullptr, msg.c_str(), "Unsupported Feature", MB_ICONERROR);
	}
	catch (std::exception& ex)
	{
		MessageBox(nullptr, ex.what(), "Unhandled Exception", MB_ICONERROR);
	}
	catch (...)
	{
		std::cerr << "Unknown exception" << std::endl;
	}

    return EXIT_FAILURE;
}
