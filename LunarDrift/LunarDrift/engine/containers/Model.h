#pragma once

#include "../graphics/buffers/VertexArrayObject.h"
#include <glm/glm.hpp>
#include <memory>

/** @brief Object-orientated version of OpenGL primitive constants */
enum class PrimitiveMode
{
    Points = GL_POINTS,
    LineStrip = GL_LINE_STRIP,
    LineLoop = GL_LINE_LOOP,
    Lines = GL_LINES,
    LineStripAdj = GL_LINE_STRIP_ADJACENCY,
    LinesAdj = GL_LINES_ADJACENCY,
    TriangleStrip = GL_TRIANGLE_STRIP,
    TriangleFan = GL_TRIANGLE_FAN,
    Triangles = GL_TRIANGLES,
    TriangleStripAdj = GL_TRIANGLE_STRIP_ADJACENCY,
    TrianglesAdj = GL_TRIANGLES_ADJACENCY,
    Patches = GL_PATCHES
};

/**
 * @brief Container for and loader of "model data" (a combination of vertex and texture data)
 *
 * @pre    A valid OpenGL context must be present to the program
 * @author Hayley Hatton
 * @date   06/03/2016
 * @see    VertexBufferObject
 * @see    ShaderProgram
 * @see    Texture
 * @see    VertexArrayObject
 * @ingroup Graphics
 */
class Model
{
public:
    /** @brief Predefined vertex format for standard 3D models */
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
        glm::vec2 texCoord;
    };

    /**
     * @brief Constructor for loading a model from an OBJ file
     * @param context Graphical context
     * @param path Path to the OBJ file to load model data from
     */
    Model(Context* context, const std::string& path);

    /**
     * @brief Constructor for a model using predefined vertex list
     * @param context Graphical context
     * @param vertices List of vertices to make model from
     */
    Model(Context* context, const std::vector<Vertex>& vertices);

    /**
     * @brief Constructor for a model using predefined vertex and index lists
     * @param context Graphical context
     * @param vertices List of vertices to make model from
     * @param indices List of indices to connect vertices with
     */
    Model(
        Context* context,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned short>& indices);

    /** @brief Default destructor */
    ~Model();


    /**
     * @brief Draw the model using a given shader
     * @param context Graphics context
     * @param shader Shader to draw the model with
     */
    void draw(Context* context, std::shared_ptr<ShaderProgram> shader) const;

    /**
     * @brief Set the typology of the model geometry for rendering
     * @param mode Typology class
     */
	void setPrimitiveMode(PrimitiveMode mode) { m_PrimitiveMode = mode; }

	/**
	* @brief Associate a given vertex array object with the model
	* @param vao Vertex array object to associate with
	*/
	void setVertexArrayObject(std::shared_ptr<VertexArrayObject> vao);

	/**
	* @brief Access associated vertex array object
	* @return Associated vertex array object
	*/
	std::weak_ptr<VertexArrayObject> getVertexArrayObject() const;

private:
	std::shared_ptr<VertexArrayObject> m_VAO;   //!< Associated vertex array object
    PrimitiveMode m_PrimitiveMode;  //!< Typology of the model geometry

    /**
     * @brief Compile model data into OpenGL objects for managing them GPU-side
     * @param context Graphical context
     * @param vertices Vector of model vertex data
     * @param indices Ptr to vector of model index data (optional)
     */
    void transferToGPU(
        Context* context,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned short>* const indices = nullptr);

    /**
     * @brief Fill a vector with vertex attributes to map the inputs to shaders
     * @param attribs Pointer to vector map of vertex attributes
     */
    void getVertexAttributes(std::vector<VertexAttribute>* attribs) const;
};
