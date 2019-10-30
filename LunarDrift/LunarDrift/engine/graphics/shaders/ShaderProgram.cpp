#include "ShaderProgram.h"
#include "../../exceptions/OpenGLException.h"
#include "../../exceptions/ShaderException.h"
#include "../../exceptions/FileIOException.h"
#include "../../Config.h"
#include <cassert>
#include <fstream>

ShaderProgram::ShaderProgram(
    Context* context,
    const std::string& vertex,
    const std::string& fragment)
{
    assert(context);
    load(context, vertex, fragment);
}

ShaderProgram::ShaderProgram(
    Context* context,
    const std::string& vertex,
    const std::string& geometry,
    const std::string& fragment)
{
    assert(context);
    load(context, vertex, geometry, fragment);
}

ShaderProgram::~ShaderProgram()
{
    unload();
}

void ShaderProgram::load(
    Context* context, 
    const std::string& vertex, 
    const std::string& fragment)
{
    assert(context);
    assert(vertex.length() > 0);
    assert(fragment.length() > 0);

    m_VertexShaderHandle = loadAndCompileFile(context, vertex, GL_VERTEX_SHADER);
    m_FragmentShaderHandle = loadAndCompileFile(context, fragment, GL_FRAGMENT_SHADER);

    m_ProgramHandle = glCreateProgram();
    if(m_ProgramHandle == 0)
        throw OpenGLException("Could not create shader program", glGetError());

    glAttachShader(m_ProgramHandle, m_VertexShaderHandle);
    glAttachShader(m_ProgramHandle, m_FragmentShaderHandle);

    glLinkProgram(m_ProgramHandle);

    // Get the link status.
    int linkStatus;
    glGetProgramiv(m_ProgramHandle, GL_LINK_STATUS, &linkStatus);

    // If the link failed, delete the program.
    if(linkStatus == GL_FALSE)
    {
        GLint logLen;
        glGetProgramiv(m_ProgramHandle, GL_INFO_LOG_LENGTH, &logLen);

		char* logBuffer = new char[logLen];
		glGetProgramInfoLog(m_ProgramHandle,
                            logLen,
                            &logLen,
                            logBuffer);
        const std::string logStr(logBuffer);
        delete logBuffer;

		throw ShaderException(vertex + " and " + fragment,
                              "shader program",
                              "linking",
                              logStr);
    }

    m_GeometryShaderHandle = 0;
}

void ShaderProgram::load(
    Context* context,
    const std::string& vertex,
    const std::string& geometry,
    const std::string& fragment)
{
    assert(context);
    assert(vertex.length() > 0);
    assert(geometry.length() > 0);
    assert(fragment.length() > 0);

    m_VertexShaderHandle = loadAndCompileFile(context, vertex, GL_VERTEX_SHADER);
    m_GeometryShaderHandle = loadAndCompileFile(context, geometry, GL_GEOMETRY_SHADER);
    m_FragmentShaderHandle = loadAndCompileFile(context, fragment, GL_FRAGMENT_SHADER);

    m_ProgramHandle = glCreateProgram();
    if(m_ProgramHandle == 0)
        throw OpenGLException("Could not create shader program", glGetError());

    glAttachShader(m_ProgramHandle, m_VertexShaderHandle);
    glAttachShader(m_ProgramHandle, m_GeometryShaderHandle);
    glAttachShader(m_ProgramHandle, m_FragmentShaderHandle);

    glLinkProgram(m_ProgramHandle);

    // Get the link status.
    int linkStatus;
    glGetProgramiv(m_ProgramHandle, GL_LINK_STATUS, &linkStatus);

    // If the link failed, delete the program.
    if(linkStatus == GL_FALSE)
    {
        GLint logLen;
        glGetProgramiv(m_ProgramHandle, GL_INFO_LOG_LENGTH, &logLen);

		char* logBuffer = new char[logLen];
		glGetProgramInfoLog(m_ProgramHandle,
                            logLen,
                            &logLen,
                            logBuffer);
        const std::string logStr(logBuffer);
        delete logBuffer;

		throw ShaderException(vertex + ", " + geometry + " and " + fragment,
                              "shader program",
                              "linking",
                              logStr);
    }
}

GLint ShaderProgram::loadAndCompileFile(
    Context* context, 
    const std::string& name, 
    GLenum stage)
{
    assert(context);
    assert(name.length() > 0);

    // Load code from file
    std::string shaderCode;
    readShaderFile(name, &shaderCode);

    int shaderLength = shaderCode.length();
    const char* szShader = shaderCode.c_str();

    // Create shader instance on GPU
    GLint shaderHandle = glCreateShader(stage);

    // Upload code to GPU and compile
    glShaderSource(shaderHandle, 1, (const char**)&szShader, &shaderLength);
    glCompileShader(shaderHandle);

    // Check if compilation was successful
    GLint status;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE) // Compilation failed
    {
		GLint logLen;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &logLen);

		char* logBuffer = new char[logLen];
		glGetShaderInfoLog(shaderHandle, logLen, &logLen, logBuffer);
        const std::string logStr(logBuffer);
        delete logBuffer;

        std::string strStage;
        if(stage == GL_VERTEX_SHADER)
            strStage = "vertex shader";
        else if(stage == GL_GEOMETRY_SHADER)
            strStage = "geometry shader";
        else if(stage == GL_TESS_CONTROL_SHADER)
            strStage = "tesselation control shader";
        else if(stage == GL_TESS_EVALUATION_SHADER)
            strStage = "tesselation evaluation shader";
        else if(stage == GL_FRAGMENT_SHADER)
            strStage = "fragment shader";
        else if(stage == GL_COMPUTE_SHADER)
            strStage = "compute shader";
        else
            strStage = "unknown shader";

		throw ShaderException(name, strStage, "compilation", logStr);
	}

	return shaderHandle;
}

GLint ShaderProgram::getAttributeLocation(
    Context* context,
    const std::string & name) const
{
    assert(context);
    return glGetAttribLocation(m_ProgramHandle, name.c_str());
}

GLint ShaderProgram::getUniformLocation(
    Context* context,
    const std::string & name) const
{
    assert(context);
    return glGetUniformLocation(m_ProgramHandle, name.c_str());
}

void ShaderProgram::unload()
{
    glDeleteShader(m_VertexShaderHandle);
    glDeleteShader(m_GeometryShaderHandle);
    glDeleteShader(m_FragmentShaderHandle);

    glDeleteProgram(m_ProgramHandle);
}

void ShaderProgram::readShaderFile(
    const std::string& filename,
    std::string* data)
{
    assert(filename.length() > 0);

    const std::string path = Config::get().get("_engine", "shaderPath");

    std::ifstream file(path + "/" + filename);

    if(!file.is_open())
        throw FileIOException(filename, "could not be opened.");

    file.seekg(0, std::ios::end);
    unsigned int size = static_cast<unsigned int>(file.tellg());
    data->reserve(size);
    file.seekg(0, std::ios::beg);

    data->assign(
        (std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
}
