#include "KesselRunRaceScene.h"
#include"../../../game/scenes/alphaspace/TestStarfield.h"
#include "hoops/HoopBlue.h"
#include "hoops/HoopRed.h"
#include "hoops/HoopGreen.h"
#include "hoops/HoopYellow.h"
#include "hoops/HoopWhite.h"
#include "../../../engine/audio/AudioPlayer.h"
#include "../../../engine/Config.h"

KesselRunRaceScene::KesselRunRaceScene(Context* context, const GameParameters& params)
	: RaceScene(context, params, "kesselRunSkybox")
{
	AudioPlayer::getInstance().setActiveMusic("Object Type 3 (Main Theme).mp3");

	placeHoops(context);

	addObject(std::make_shared<TestStarfield>(context, getManagers()));
}

KesselRunRaceScene::~KesselRunRaceScene()
{

}


void KesselRunRaceScene::placeHoops(Context* context)
{
	// Hoops
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(0.f, 0.f, -3.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(0.f, -5.f, -20.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(0.f, -5.f, -28.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(0.f, -10.f, -40.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(0.f, -12.f, -60.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(0.f, -10.f, -70.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(0.f, -10.f, -85.f), glm::vec2(0.0f, 80.0f)));
	addObject(std::make_shared<HoopBlue>(context, getManagers(), glm::vec3(10.f, -10.f, -100.f), glm::vec2(0.0f, 120.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(25.f, -8.f, -100.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopRed>(context, getManagers(), glm::vec3(38.f, -8.f, -90.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(45.f, 0.f, -60.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopGreen>(context, getManagers(), glm::vec3(55.f, 0.f, -60.f), glm::vec2(0.0f, 0.0f)));
	addObject(std::make_shared<HoopYellow>(context, getManagers(), glm::vec3(55.f, 0.f, -80.f), glm::vec2(0.0f, 40.0f)));
	addObject(std::make_shared<HoopWhite>(context, getManagers(), glm::vec3(90.f, 0.f, -60.f), glm::vec2(0.0f, 0.0f)));
}

std::shared_ptr<Camera> KesselRunRaceScene::getBirdsEyeCam(Context* context, const glm::vec3& pos) const
{
	const GLfloat fovy = 70.f;
	GLfloat nearZ = Config::get().getFloat("config", "camera:nearZ");
	GLfloat farZ = Config::get().getFloat("config", "camera:farZ");

	GLsizei width, height;
	context->getDimensions(&width, &height); 

	std::shared_ptr<Camera> camera = std::make_shared<Camera>(
		width, height, nearZ, farZ, fovy); 
	camera->lookAt(
		glm::vec3(20.f, 50.f, 0.f),
		glm::vec3(pos),
		glm::vec3(0.f, 0.f, -1.f));

	return camera;
}