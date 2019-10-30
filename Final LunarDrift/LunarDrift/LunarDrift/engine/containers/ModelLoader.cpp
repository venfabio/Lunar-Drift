#include "ModelLoader.h"
#include "../Config.h"
#include "../exceptions/FileIOException.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

ModelLoader::ModelLoader(const std::string & path)
{
    load(path);
}

ModelLoader::~ModelLoader()
{
    //dtor
}


void ModelLoader::load(const std::string& path)
{
    const std::string modelPath = Config::get().get("_engine", "modelPath");
    const std::string adjustedPath(modelPath + "/" + path + ".obj");
    std::ifstream modelFile(adjustedPath);
    std::map<std::string, glm::vec3> materials;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;


    if(!modelFile.is_open())
        throw FileIOException(adjustedPath, "could not be opened.");

    std::string group = "", material = "";
    char reserved[255];
    while(!modelFile.eof())
    {
        std::string key;
        modelFile >> key;

        if(!key.compare("f"))
        {
            for(int i = 0; i < 3; ++i)
            {
                Vertex vertex;

                std::string raw;
                modelFile >> raw;

                // Extract sub-components
				int lastPos = 0;
				std::size_t pos = 0;
				for (unsigned int j = 0; j < 3; ++j)
                {
					pos = raw.find_first_of('/', lastPos);
					std::string field = raw.substr(lastPos, pos - lastPos);
					lastPos = pos + 1;

                    if(j == 0)
                    {
                        std::stringstream ss(field);
                        int index;
                        ss >> index;
                        --index;

                        vertex.position = vertices[index];
                    } else if(j == 1)
                    {
                        std::stringstream ss(field);
                        int index;
                        ss >> index;
                        --index;

                        vertex.texCoord = texcoords[index];
                    } else if(j == 2)
                    {
                        std::stringstream ss(field);
                        int index;
                        ss >> index;
                        --index;

                        vertex.normal = normals[index];
                    }
                }

                if(material.length() > 0)
                {
                    vertex.color = materials[material];
                } else {
                    vertex.color.r = 0.f;
                    vertex.color.g = 0.f;
                    vertex.color.b = 0.f;
                }

                m_VertexGroups[group].push_back(vertex);
            }
        }
        else if(!key.compare("v"))
        {
            glm::vec3 vertex;
            modelFile >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        else if(!key.compare("vn"))
        {
            glm::vec3 normal;
            modelFile >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if(!key.compare("vt"))
        {
            glm::vec2 texcoord;
            modelFile >> texcoord.x >> texcoord.y;
            texcoords.push_back(texcoord);
        }
        else if(!key.compare("g"))
        {
            modelFile >> group;
            material = "";
        }
        else if(!key.compare("usemtl"))
        {
            modelFile >> material;
        }
        else if(!key.compare("mtllib"))
        {
            std::string mtlPath;
            modelFile >> mtlPath;
            loadMaterials(materials, modelPath + "/" + mtlPath);
        }
        else if(key.length() > 0)
            modelFile.getline(reserved, 255);
    }
}

int ModelLoader::getNumVertices() const
{
    int size = 0;
    std::map<std::string, std::vector<Vertex> >::const_iterator group;

    for(group = m_VertexGroups.begin(); group != m_VertexGroups.end(); ++group)
        size += group->second.size();

    return size;
}

bool ModelLoader::compile(int stride,
                          int positionOffset,
                          int normalOffset,
                          int texOffset,
                          int colorOffset,
                          int* elements,
                          void** output) const
{
    std::map<std::string, std::vector<Vertex> >::const_iterator group;
    unsigned int vertexOffset = 0;
    int size = getNumVertices();
    unsigned char* data = new unsigned char[size * stride];

    unsigned int positionSize = 3;
    unsigned int normalSize = 3;
    unsigned int texCoordSize = 2;
    unsigned int colorSize = 3;

    for(group = m_VertexGroups.begin(); group != m_VertexGroups.end(); ++group)
    {
        const std::vector<Vertex> g = group->second;
        unsigned int numElements = g.size();

        for(unsigned int i = 0; i < numElements; ++i)
        {
            Vertex vertex = g[i];

            if(positionOffset != -1)
                memcpy(data + vertexOffset + positionOffset,
                       &vertex.position, sizeof(float) * positionSize);
            if(normalOffset != -1)
                memcpy(data + vertexOffset + normalOffset,
                       &vertex.normal, sizeof(float) * normalSize);
            if(texOffset != -1)
                memcpy(data + vertexOffset + texOffset,
                       &vertex.texCoord, sizeof(float) * texCoordSize);
            if(colorOffset != -1)
                memcpy(data + vertexOffset + colorOffset,
                       &vertex.color, sizeof(float) * colorSize);

            vertexOffset += stride;
        }
    }

    *elements = size;
    *output = data;

    return true;
}

void ModelLoader::loadMaterials(
    std::map<std::string, glm::vec3> & buffer,
    const std::string & filepath) const
{
    std::ifstream file(filepath.c_str());

    if(!file.is_open())
        return;

    std::string material = "";
    char reserved[255];
    while(!file.eof())
    {
        std::string key;
        file >> key;
        if(!key.compare("newmtl"))
            file >> material;
        else if(!key.compare("Kd"))
        {
            if(material.length() > 0)
            {
                glm::vec3 color;
                file >> color.x >> color.y >> color.z;
                buffer[material] = color;
            }
        }
        else if(key.length() > 0)
            file.getline(reserved, 255);
    }
}
