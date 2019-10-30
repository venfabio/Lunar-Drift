#include "Model.h"
#include "ModelLoader.h"
#include "../graphics/buffers/IndexBufferObject.h"
#include <cstddef>
#include <memory>

Model::Model(Context* context, const std::string& path)
    : m_PrimitiveMode(PrimitiveMode::Triangles)
{
    // Load the model into a generic intermediate container
    ModelLoader ml(path);

    // Extract vertices from intermediate container into raw heap memory
    Vertex* rawVertices;
    int numVertices = ml.getNumVertices();
    ml.compile(
        sizeof(Vertex),
        offsetof(Vertex, position),
        offsetof(Vertex, normal),
        offsetof(Vertex, texCoord),
        offsetof(Vertex, color),
        &numVertices,
        (void**)(&rawVertices));

    // And now transfer from raw heap memory into nice and proper STL containers
    std::vector<Vertex> vertices(rawVertices, rawVertices + numVertices);
    delete[] rawVertices;   // We're done with you!

    for (Vertex& v : vertices)
        v.color.a = 1.0f;

    // Now send to OpenGL objects
    transferToGPU(context, vertices, nullptr);
}

Model::Model(Context* context, const std::vector<Vertex>& vertices)
    : m_PrimitiveMode(PrimitiveMode::Triangles)
{
    transferToGPU(context, vertices, nullptr);
}

Model::Model(
    Context* context,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned short>& indices)
    : m_PrimitiveMode(PrimitiveMode::Triangles)
{
    transferToGPU(context, vertices, &indices);
}

Model::~Model()
{
    //dtor
}


void Model::transferToGPU(
    Context* context,
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned short>* const indices)
{
    // Transfer from STL into GPU via VBOs
    std::shared_ptr<VertexBufferObject<Vertex>> vbo
        = std::make_shared<VertexBufferObject<Vertex>>(context, vertices);

    // This points to nothing by default
    std::shared_ptr<IndexBufferObject> ibo;
    // If we have indices, create an IBO for it to point to
    if(indices != nullptr)
        ibo = std::make_shared<IndexBufferObject>(context, *indices);

    // Need the attribute map to describe the vertex to the GPU
    std::vector<VertexAttribute> attribs;
    getVertexAttributes(&attribs);

    // Now create the final VAO to wrap all of the above together :)
    std::shared_ptr<VertexArrayObject> vao
        = std::make_shared<VertexArrayObject>(context, vbo, attribs, ibo);

    setVertexArrayObject(vao);
}

void Model::getVertexAttributes(std::vector<VertexAttribute>* attribs) const
{
    attribs->push_back(VertexAttribute(
        "Position",
        offsetof(Vertex, position),
        sizeof(Vertex),
        VertexAttribute::Type::Float,
        3));
    attribs->push_back(VertexAttribute(
        "Normal",
        offsetof(Vertex, normal),
        sizeof(Vertex),
        VertexAttribute::Type::Float,
        3));
    attribs->push_back(VertexAttribute(
        "Color",
        offsetof(Vertex, color),
        sizeof(Vertex),
        VertexAttribute::Type::Float,
        4));
    attribs->push_back(VertexAttribute(
        "TexCoord",
        offsetof(Vertex, texCoord),
        sizeof(Vertex),
        VertexAttribute::Type::Float,
        2));
}

void Model::setVertexArrayObject(std::shared_ptr<VertexArrayObject> vao)
{
	m_VAO = vao;
}

std::weak_ptr<VertexArrayObject> Model::getVertexArrayObject() const
{
	return m_VAO;
}

void Model::draw(
    Context* context,
    std::shared_ptr<ShaderProgram> shader) const
{
    GLenum errCode;
    assert((errCode = glGetError()) == GL_NO_ERROR);

    std::shared_ptr<VertexArrayObject> vao = getVertexArrayObject().lock();
    vao->bind(context, shader);

    if(vao->getIndexBuffer() != nullptr)
    {
        std::shared_ptr<IndexBufferObject> ibo = vao->getIndexBuffer();
        glDrawElements(
            static_cast<GLenum>(m_PrimitiveMode),
            ibo->size(),
            ibo->getOpenGLClass(),
            (const void*)(0));
    } else {
        glDrawArrays(
            static_cast<GLenum>(m_PrimitiveMode),
            0,
            vao->getVertexBuffer()->size());
    }

    assert((errCode = glGetError()) == GL_NO_ERROR);
}
