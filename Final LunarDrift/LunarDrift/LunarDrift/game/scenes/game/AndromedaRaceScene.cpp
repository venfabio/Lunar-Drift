#include "AndromedaRaceScene.h"
#include"../../../game/scenes/alphaspace/TestStarfield.h"
#include "hoops/HoopBlue.h"
#include "hoops/HoopRed.h"
#include "hoops/HoopGreen.h"
#include "hoops/HoopYellow.h"
#include "hoops/HoopWhite.h"
#include "environment\Planet.h"
#include "../../../engine/audio/AudioPlayer.h"
#include "../../../engine/Config.h"

AndromedaRaceScene::AndromedaRaceScene(Context* context, const GameParameters& params)
	: RaceScene(context, params, "andromedaSkybox")
{
	AudioPlayer::getInstance().setActiveMusic("Hurricane Dimension.mp3");

	placeHoops(context);

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
}

AndromedaRaceScene::~AndromedaRaceScene()
{

}


void AndromedaRaceScene::placeHoops(Context* context)
{
	// Hoops
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(0.f, -2.f, -3.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(0.f, -2.f, -10.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(0.f, -2.f, -30.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(-10.f, 4.f, -38.f), glm::vec2(0.0f, 40.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(-23.f, 6.f, -38.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(-38.f, 0.f, -38.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(-50.f, 0.f, -30.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(-60.f, 0.f, -20.f), glm::vec2(0.0f, -40.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(-60.f, 0.f, -10.f), glm::vec2(0.0f, -30.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(-45.f, 0.f, 10.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(-20.f, -4.f, 30.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopWhite>(context, getManagers(), glm::vec3(-10.f, -8.f, 30.f), glm::vec2(0.0f, 0.0f)));
}

std::shared_ptr<Camera> AndromedaRaceScene::getBirdsEyeCam(Context* context, const glm::vec3& pos) const
{
	const GLfloat fovy = 70.f;
	GLfloat nearZ = Config::get().getFloat("config", "camera:nearZ");
	GLfloat farZ = Config::get().getFloat("config", "camera:farZ");

	GLsizei width, height;
	context->getDimensions(&width, &height); 

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(
		width, height, nearZ, farZ, fovy); 
	camera->lookAt(
		glm::vec3(40.f, 50.f, -10.f),
		glm::vec3(pos),
		glm::vec3(0.f, 0.f, -1.f));

	return camera;
}