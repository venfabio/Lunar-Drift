#pragma once

#include "../containers/SceneObject.h"
#include "../containers/Model.h"
#include "../graphics/CubeMapTexture.h"

/**
 * @brief OpenGL+GLSL skybox
 *
 * This is a simple inversed cube mapped to the viewport such that it
 * is infinitely distant despite being so close, so long as it's rendered
 * behind everything!

 * @pre     A valid OpenGL context must be present to the program.
 * @author  Hayley Hatton
 * @date    26/05/2016
 * @ingroup Prefabs
 */
class Skybox : public SceneObject
{
public:
	/**
	* @brief Default constructor
	* @param context Graphics context
	* @param managers Weak reference to scene's metamanager
	* @param texDir Which subdirectory in the textures folder holds the skybox textures
	*/
	Skybox(
		Context* context, 
		std::weak_ptr<MetaManager> managers,
		const std::string& texDir);

    /** Default destructor */
    virtual ~Skybox();


	/**
	 * @brief Draw the skybox in its current state
	 * @param context Graphics context
	 * @param shaders Shader manager containing shader programs for use
	 */
	void draw(Context* context, std::shared_ptr<ShaderManager> shaders) override;


private:
	std::unique_ptr<VertexArrayObject> m_VAO;
	std::unique_ptr<CubeMapTexture> m_CubeMapTex;

	/**
	 * @brief Get vertex attributes for mapping vertex data to shader pipeline
	 * @param layout Pointer to buffer to push vertex attributes into
	 */
	void getLayoutAttribs(std::vector<VertexAttribute>* layout) const;
};

