#pragma once

#include "RaceScene.h"

/**
 * @brief Scene class for Antares track, the easy-tier track
 *
 * @author Hayley Hatton, Fabio, Mariana
 * @date   26/05/2016
 * @ingroup Game
 */
class AntaresRaceScene : public RaceScene
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
	 * @param params Previously selected parameters via menus for the game to use for dynamic play
     */
	AntaresRaceScene(Context* context, const GameParameters& params);

    /** @brief Default destructor */
    ~AntaresRaceScene();


protected:
	/**
	 * @brief Called when the birds eye view camera for the scene is needed
     * @param context Graphics context
	 * @param event Smart pointer to the appropriate camera
	 */
	std::shared_ptr<Camera> getBirdsEyeCam(Context* context, const glm::vec3& pos) const override;

	/**
	* @brief Called to place hoops into the scene
	* @param context Graphics context
	*/
	void placeHoops(Context* context);
};
