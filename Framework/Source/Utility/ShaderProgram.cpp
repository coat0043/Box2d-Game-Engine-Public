#include "FrameworkPCH.h"

#include "ShaderProgram.h"
#include "Helpers.h"

namespace fw {

ShaderProgram::ShaderProgram()
{
    m_VertShaderString = nullptr;
    m_FragShaderString = nullptr;

    m_VertShader = 0;
    m_FragShader = 0;
    m_Program = 0;
}

ShaderProgram::ShaderProgram(const char* vertFilename, const char* fragFilename)
{
    m_VertShaderString = nullptr;
    m_FragShaderString = nullptr;

    m_VertShader = 0;
    m_FragShader = 0;
    m_Program = 0;

    Init(vertFilename, fragFilename);
}

ShaderProgram::~ShaderProgram()
{
    Cleanup();
}

void ShaderProgram::Cleanup()
{
    if (m_VertShaderString)
        delete[] m_VertShaderString;
    if (m_FragShaderString)
        delete[] m_FragShaderString;

    glDetachShader(m_Program, m_VertShader);
    glDetachShader(m_Program, m_FragShader);

    if (m_VertShader)
        glDeleteShader(m_VertShader);
    if (m_FragShader)
        glDeleteShader(m_FragShader);
    if (m_Program)
        glDeleteProgram(m_Program);

    m_VertShaderString = nullptr;
    m_FragShaderString = nullptr;

    m_VertShader = 0;
    m_FragShader = 0;
    m_Program = 0;
}

void ShaderProgram::CompileShader(GLuint& shaderHandle, const char* shaderString)
{
    const char* functionsString = LoadCompleteFile("Data/Shaders/Functions.glsl", nullptr);

    // Some correction to pull #version tag from the shader text to the beginning of the functions.glsl string if necessary.

    std::string correctedShaderString = shaderString;
    std::string correctedFunctionsString = functionsString;

    {
        std::string versionText = shaderString;
        versionText = versionText.substr(0, 8);

        if (versionText == "#version") // if we're using a #version directive, move it from the .frag string to the .glsl string
        {
            int newlineIndex = (int)correctedShaderString.find("\n") + 1;
            versionText = correctedShaderString.substr(0, newlineIndex);
            correctedShaderString = correctedShaderString.erase(0, newlineIndex);
            correctedFunctionsString.insert(0, versionText);
        }
    }

    const char* strings[] = {correctedFunctionsString.c_str(), correctedShaderString.c_str()};

    glShaderSource(shaderHandle, 2, strings, nullptr);

    glCompileShader(shaderHandle);
    delete[] functionsString;

    //GLenum errorcode = glGetError();

    int compiled = 0;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compiled);
    if (compiled == 0)
    {
        int infolen = 0;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &infolen);

        char* infobuffer = new char[infolen + 1];
        glGetShaderInfoLog(shaderHandle, infolen + 1, nullptr, infobuffer);
        OutputMessage(infobuffer);
        assert(false);
        delete[] infobuffer;

        glDeleteShader(shaderHandle);
        shaderHandle = 0;
    }
}

bool ShaderProgram::Init(const char* vertFilename, const char* fragFilename)
{
    m_VertShaderString = LoadCompleteFile(vertFilename, nullptr);
    m_FragShaderString = LoadCompleteFile(fragFilename, nullptr);

    assert(m_VertShaderString != nullptr && m_FragShaderString != nullptr);
    if (m_VertShaderString == nullptr || m_FragShaderString == nullptr)
        return false;

    return Reload();
}

bool ShaderProgram::Reload()
{
    assert(m_VertShaderString != nullptr);
    assert(m_FragShaderString != nullptr);

    m_VertShader = glCreateShader(GL_VERTEX_SHADER);
    m_FragShader = glCreateShader(GL_FRAGMENT_SHADER);

    CompileShader(m_VertShader, m_VertShaderString);
    CompileShader(m_FragShader, m_FragShaderString);

    if (m_VertShader == 0 || m_FragShader == 0)
    {
        Cleanup();
        return false;
    }

    m_Program = glCreateProgram();
    glAttachShader(m_Program, m_VertShader);
    glAttachShader(m_Program, m_FragShader);

    glLinkProgram(m_Program);

    int linked = 0;
    glGetProgramiv(m_Program, GL_LINK_STATUS, &linked);
    if (linked == 0)
    {
        int infolen = 0;
        glGetProgramiv(m_Program, GL_INFO_LOG_LENGTH, &infolen);

        char* infobuffer = new char[infolen + 1];
        glGetProgramInfoLog(m_Program, infolen + 1, nullptr, infobuffer);
        OutputMessage(infobuffer);
        assert(false);
        delete[] infobuffer;

        Cleanup();
        return false;
    }

    return true;
}

} // namespace fw
