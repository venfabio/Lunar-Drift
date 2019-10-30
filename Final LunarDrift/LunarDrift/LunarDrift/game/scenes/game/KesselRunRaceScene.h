#pragma once

#include "RaceScene.h"

/**
 * @brief Scene class for Kessel Run track, the hard-tier track
 *
 * @author Hayley Hatton, Fabio, Mariana
 * @date   26/05/2016
 * @note Can yoooou complete the Kessel Run in under 12 parsecs? ;)
 * @ingroup Game
 */
class KesselRunRaceScene : public RaceScene
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
	 * @param params Previously selected parameters via menus for the game to use for dynamic play
     */
	KesselRunRaceScene(Context* context, const GameParameters& params);

    /** @brief Default destructor */
	~KesselRunRaceScene();


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
