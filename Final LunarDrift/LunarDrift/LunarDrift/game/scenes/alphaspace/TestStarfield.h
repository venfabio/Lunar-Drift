#pragma once

#include "../../../engine/containers/SceneObject.h"
#include "../../../engine/containers/Model.h"
#include "../../../engine/comms/IMessageListener.h"

/**
 * @brief Example starfield, showing use of procedurally-generated point cloud
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   16/03/2016
 * @ingroup TestScene
 */
class TestStarfield : public SceneObject
{
public:
    /** @brief Default constructor */
    TestStarfield(Context* context, std::weak_ptr<MetaManager> managers);

    /** @brief Default destructor */
    ~TestStarfield();


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
     * @brief Draw the object in its current state
     * @param context Graphics context
     * @param shaders Shader manager containing shader programs for use
     */
    void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;

protected:
    void generateStars(std::vector<Model::Vertex>* vertices) const;

private:
    std::shared_ptr<Model> m_Stars;
};
