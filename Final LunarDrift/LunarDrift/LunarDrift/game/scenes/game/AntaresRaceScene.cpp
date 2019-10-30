#include "AntaresRaceScene.h"
#include"../../../game/scenes/alphaspace/TestStarfield.h"
#include "hoops/HoopBlue.h"
#include "hoops/HoopRed.h"
#include "hoops/HoopGreen.h"
#include "hoops/HoopYellow.h"
#include "hoops/HoopWhite.h"
#include "environment\Mars.h"
#include "../../../engine/audio/AudioPlayer.h"
#include "../../../engine/Config.h"

AntaresRaceScene::AntaresRaceScene(Context* context, const GameParameters& params)
	: RaceScene(context, params, "antaresSkybox")
{
	AudioPlayer::getInstance().setActiveMusic("Inferno Unleashed.mp3");

	placeHoops(context);

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
}

AntaresRaceScene::~AntaresRaceScene()
{

}


void AntaresRaceScene::placeHoops(Context* context)
{
	// Hoops
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(0.f, 0.f, -3.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(0.f, 0.f, -10.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(0.f, 0.f, -18.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(0.f, 4.f, -24.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(9.f, 4.f, -36.f), glm::vec2(0.0f, 120.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(20.f, 6.f, -36.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(30.f, 6.f, -36.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(40.f, 2.f, -36.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(55.f, 0.f, -36.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(60.f, 0.f, -45.f), glm::vec2(0.0f, 120.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(60.f, -2.f, -60.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(60.f, -8.f, -75.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(50.f, -15.f, -85.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(60.f, -15.f, -95.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopWhite>(context, getManagers(), glm::vec3(60.f, -15.f, -120.f), glm::vec2(0.0f, 80.0f)));

	// Environment stuff
	addObject(std::make_shared<Mars>(context, getManagers(), glm::vec3(20.f, 6.f, -65.f)));
}

std::shared_ptr<Camera> AntaresRaceScene::getBirdsEyeCam(Context* context, const glm::vec3& pos) const
{
	const GLfloat fovy = 70.f;
	GLfloat nearZ = Config::get().getFloat("config", "camera:nearZ");
	GLfloat farZ = Config::get().getFloat("config", "camera:farZ");

	GLsizei width, height;
	context->getDimensions(&width, &height); 

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(
		width, height, nearZ, farZ, fovy); 
	camera->lookAt(
		glm::vec3(10.f, -30.f, 30.f),
		glm::vec3(pos),
		glm::vec3(0.f, 0.f, -1.f));

	return camera;
}