#pragma once

#include "../containers/SceneObject.h"
#include "../graphics/buffers/VertexArrayObject.h"
#include "../graphics/Texture.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @brief A prefabricated container for displaying text on the screen
 *
 * @author   Hayley Hatton
 * @date     20/03/2016
 * @ingroup Prefabs
 */
class Text : public SceneObject
{
public:
    /**
     * @brief Constructor
     * @param context Graphics context
     * @param managers Weak reference to scene's metamanager
     * @param pos Position on the screen to position the text
     * @param text Initial text to set the label to
	 * @param updateInDraw Indicate to sync with GPU every draw
     */
	Text(
        Context* context,
		std::weak_ptr<MetaManager> managers,
		glm::vec2& pos = glm::vec2(0.f, 0.f),
		const std::string& text = "",
		bool updateInDraw = false);

    /** @brief Default destructor */
	virtual ~Text();


    /**
     * @brief Set the position of the text on the screen
     * @param pos Position of top-left coordinate in screen coordinates
     */
	void setPosition(const glm::vec2& pos);

    /**
     * @brief Set the text of the internal buffer
     * @param text New text to overwrite previous buffer state with
     */
	void setText(const std::string& text);

    /**
     * @brief Set the text of the internal buffer with an integer
     * @param number Integer to overwrite previous buffer state with
     */
    void setNumerical(int number);

    /**
     * @brief Set the text of the internal buffer with a double
     * @param number Double to overwrite previous buffer state with
     */
    void setNumerical(double number);

    /**
     * @brief Get the text of the internal buffer
     * @note This may not be what's on the screen! Internal buffer =/= GPU buffer
     * @return Current text of internal buffer
     */
	const std::string& getText() const;

    /**
     * @brief Push a character onto the end of the internal buffer
     * @param ch ANSI character to add
     */
	void addChar(char ch);

    /**
     * @brief Remove character from the end of the internal buffer
     */
	void pop();

    /**
     * @brief Push an integer onto the end of the internal buffer
     * @param number Integer to push (will be converted to text)
     */
    void addNumerical(int number);

    /**
     * @brief Push a double onto the end of the internal buffer
     * @param number Double to push (will be converted to text)
     */
    void addNumerical(double number);

    /**
     * @brief Get the width of a given text string when represented in this
     * @param text Text string to test width of
     * @return Width of text string in screen pixels
     */
    static unsigned int getTextWidth(const std::string& text);

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
     * @brief Draw the text in its current state
     * @param context Graphics context
     * @param shaders Shader manager containing shader programs for use
     */
    void draw(Context* context, std::shared_ptr<ShaderManager> shaders);

private:
    /**
     * @brief A vertex specialized for efficient text rendering
     */
	struct VERTEX
	{
        glm::vec2 position;
		glm::vec2 texCoord;
	};

    /**
     * @brief Quantized letter structure, for cleaner code :)
     */
	struct Letter
	{
		VERTEX vertices[6];
	};

    std::vector<Letter> m_Letters;  //!< Intermediate text buffer #2
    std::unique_ptr<VertexArrayObject> m_VAO;   //!< GPU vertex buffer
    std::unique_ptr<Texture> m_Alphabet;        //!< Alphabet texture
	std::string m_Text;     //!< Intermediate text buffer #1
	bool m_NeedsUpdating;   //!< If true, next predraw we synchronize with GPU
	bool m_UpdateInDraw;	//!< If true, update in draw too!
	const unsigned int m_MaxTextLength = 80;

    /**
     * @brief Create a valid GPU vertex buffer for our text
     * @param context Graphics context
     */
	void reserveBuffer(Context* context);

    /**
     * @brief Synchronize any updates from text to GPU vertex buffer
     * @param context Graphics context
     */
	void commitUpdates(Context* context);

    /**
     * @brief Copy text from intermediate text buffer into vertex buffer
     */
	void plantTextInBuffer();

    /**
     * @brief Get the tex coordinates of a given letter
     * @return Start (vec2::x) and end (vec2::y) of the letter in alphabet tex
     */
	static glm::vec2 getLetterTexDimensions(char letter);

    /**
     * @brief Ensure text is render-compatible
     */
	void transformText();

	void getLayoutAttribs(std::vector<VertexAttribute>* attribs) const;
};
