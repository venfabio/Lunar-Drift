#pragma once

#include "SceneObjectComponent.h"
#include "../containers/Model.h"
#include "../prefabs/DebuggingSphere.h"
#include <vector>

/** @brief Collision sphere for use as part of a sphere-tree collision mesh in CollisionComponent */
struct CollisionSphere
{
    unsigned int id;    //!< ID to identify the collision point in CollisionMessage
    glm::vec3 offset;   //!< Where center of sphere is, relative to object's center
    float radius;       //!< Size of collision sphere
};

/**
 * @brief Component that describes collision information about the parent
 *
 * This is current setup to use "sphere cloud" collision geometry. In essence,
 * an object is described as a collection of spheres which approximate its
 * visual geometry.
 * Additionally, as a cheap "broad" collision test, a "Far Sphere" is modelled
 * that encompasses the entire object. Thus, rather than testing collisions
 * between potentially hundreds of CollisionSpheres, a broad test of just two
 * spheres can be used to see if a narrower test is required.
 *
 * @author Hayley Hatton
 * @date   06/03/2016
 * @see	   CollisionManager
 * @see    CollisionSphere
 * @ingroup Components
 */
class CollisionComponent : public SceneObjectComponent, 
                           public std::enable_shared_from_this<CollisionComponent>
{
public:
	/**
	 * @brief Default constructor
	 * @param parent Pointer to the scene object to attach component to
	 * @param managers Metamanager for accessing scene's managers
     * @param farRadius Furthest possible extent of collision
     * @param points List of points for spherical collision cloud
	 */
	CollisionComponent(
		SceneObject* parent, 
        std::weak_ptr<MetaManager> managers,
        float farRadius,
        const std::vector<CollisionSphere> points);

	/** @brief Default destructor */
	~CollisionComponent();


	/**
	 * @brief Step the simulation state
	 * @param dt Delta-time since last step call in seconds
	 */
	void step(double dt) override;

	/**
	 * @brief Called before the renderer draws the scene
	 * This allows the scene objects to do updates that require a
	 * graphics context before the graphical state is drawn
	 * @param context Graphics context
	 */
	void predraw(Context* context) override;

    /**
     * @brief Access the radius of the all-encompassing broad sphere
     * @return Radius of all-encompassing broad sphere in glm units
     */
    float getFarRadius() const { return m_FarRadius; }

    /**
     * @brief Access the cloud of collision spheres 
     */
    const std::vector<CollisionSphere>& getCollisionSpheres() const;

private:
	std::vector<CollisionSphere> m_CollisionSpheres;
	std::vector<std::weak_ptr<DebuggingSphere>> m_DebuggingSpheres;
    float m_FarRadius;  //!< Radius of all-encompassing sphere for broad collision testing
    bool m_Registered;  //!< Cannot shared_from_this in ctor; defer adding to CollisionManager
	bool m_Debugging;	//!< If true, show collision spheres
};
