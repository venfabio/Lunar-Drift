#pragma once

#include <string>
#include <map>
#include <vector>
#include <glm/glm.hpp>


/**
 * @brief Loads a Wavefront Object file and allows the model to be modified.
 *
 * Wavefront Object Files are free, license-free, text-based model-storing file
 * format.
 *
 * @author Hayley Hatton
 * @date   06/03/2016
 * @see    Model
 * @ingroup Graphics
 */
class ModelLoader
{
public:
    /** @brief Default constructor */
    ModelLoader() {};

    /**
     * @brief Load model from path.
     * @param path Location of model file.
     */
    ModelLoader(const std::string & path);

    /** @brief Default destructor */
    virtual ~ModelLoader();


    /**
     * @brief Load model from path.
     * @param path Location of model file.
     */
    void load(const std::string & path);

    /** @brief Return the number of vertices loaded. */
    int getNumVertices() const;

    /**
     * @brief Compile the loaded model into a custom interleaved structure
     * @param stride The size of the structure
     * @param positionOffset The offset to the position vector in the structure
     * @param normalOffset The offset to the normal vector in the structure
     * @param texOffset The offset to the texture coordinates in the structure
     * @param colorOffset The offset to the color vectors in the structure
     * @param elements Pointer to the number of elements loaded
     * @param output Pointer to store a pointer to the compiled data
     * @return Error status. True if success, false if fail
     */
    bool compile(
        int stride,
        int positionOffset,
        int normalOffset,
        int texOffset,
        int colorOffset,
        int* elements,
        void** output) const;

private:
    struct Vertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 color;
        glm::vec2 texCoord;
    };
    std::map<std::string, std::vector<Vertex> > m_VertexGroups;

    /**
     * @brief Load the color information from an external material file.
     * @param buffer Reference to map to store the material information.
     * @param filepath Location of material file.
     */
    void loadMaterials(
        std::map<std::string, glm::vec3> & buffer,
        const std::string & filepath) const;
};
