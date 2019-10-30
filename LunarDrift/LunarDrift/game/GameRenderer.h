#pragma once

#include "../engine/graphics/renderers/Renderer.h"

/**
 * @brief Customized Renderer for our game
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   16/03/2016
 * @ingroup Game
 */
class GameRenderer : public Renderer
{
public:
    /**
     * @brief Default constructor
     * @param context Graphics context
     * @param width Width of context in pixels
     * @param height Height of context in pixels
     */
	GameRenderer(Context* context, GLsizei width, GLsizei height);

    /** @brief Default destructor */
	~GameRenderer();


protected:
    /**
     * @brief Create and return a shader manager containing the standard shaders
     * @return Managed pointer of a default shader manager
     */
    std::shared_ptr<ShaderManager> loadStandardShaders(
        Context* context) override;

private:
};
