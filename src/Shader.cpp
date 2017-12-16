#include <iostream>
#include <vector>

#include "Shader.hpp"

Shader::~Shader() {
    DeleteProgram();
}

void Shader::DeleteProgram() {
    for(auto shaderId: mShaderIds)
    {
        glDeleteShader(shaderId);
    }
    glDeleteProgram(mId);
}

void Shader::Use() const {
    glUseProgram(mId);
}

GLuint Shader::GetId() const {
    return mId;
}

GLuint Shader::CreateShader(const GLenum shaderType,
                            const std::string &shaderSource) const {
    GLint compileResult{};
    GLchar log[256];
    GLuint shader{};

    shader = glCreateShader(shaderType);
    const GLchar * ptrShaderCode = shaderSource.c_str();
    const GLint shaderCodeSize = shaderSource.size();

    glShaderSource(shader, 1, &ptrShaderCode, &shaderCodeSize);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);

    if (compileResult == GL_FALSE) {
        glGetShaderInfoLog(shader, sizeof(log), 0, log);

        if (shader > 0) {
            glDeleteShader(shader);
        }

        throw std::runtime_error(shaderSource + "\n" + log);
    }
    return shader;
}

void Shader::AttachShaders(const std::string &vertexShaderCode,
                           const std::string &fragmentShaderCode,
                           const std::string &geometryShaderCode) {
    mLinked = false;

    mShaderIds.emplace_back(CreateShader(GL_VERTEX_SHADER, vertexShaderCode));
    mShaderIds.emplace_back(CreateShader(GL_FRAGMENT_SHADER, fragmentShaderCode));

    if(!geometryShaderCode.empty())
    {
        mShaderIds.emplace_back(CreateShader(GL_GEOMETRY_SHADER, geometryShaderCode));
    }

    mId = glCreateProgram();
    for(auto shaderId: mShaderIds)
    {
        glAttachShader(mId, shaderId);
    }
}

void Shader::LinkProgram()
{
    if(mLinked)
    {
        return;
    }

    mLinked = true;

    GLchar log[256];
    GLint  link_result{};

    glLinkProgram(mId);
    glGetProgramiv(mId, GL_LINK_STATUS, &link_result);

    if (link_result == GL_FALSE) {
        glGetProgramInfoLog(mId, sizeof(log), 0, log);
        std::cerr << "Shader program error: " << log << std::endl;

        throw std::runtime_error("error linking shader");
    }
}



void Shader::SetUniform(GLchar const *name, GLfloat value) {
    glUniform1f(glGetUniformLocation(mId, name), value);
}

void Shader::SetUniform(GLchar const *name, GLint value) {
    glUniform1i(glGetUniformLocation(mId, name), value);
}

void Shader::SetUniform(GLchar const *name, GLfloat x, GLfloat y) {
    glUniform2f(glGetUniformLocation(mId, name), x, y);
}

void Shader::SetUniform(GLchar const *name, glm::vec2 const &value) {
    glUniform2f(glGetUniformLocation(mId, name), value.x, value.y);
}

void Shader::SetUniform(GLchar const *name, GLfloat x, GLfloat y, GLfloat z) {
    glUniform3f(glGetUniformLocation(mId, name), x, y, z);
}

void Shader::SetUniform(GLchar const *name, glm::vec3 const &value) {
    glUniform3f(glGetUniformLocation(mId, name), value.x, value.y, value.z);
}

void Shader::SetUniform(GLchar const *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
    glUniform4f(glGetUniformLocation(mId, name), x, y, z, w);
}

void Shader::SetUniform(GLchar const *name, glm::vec4 const &value) {
    glUniform4f(glGetUniformLocation(mId, name), value.x, value.y, value.z, value.w);
}

void Shader::SetUniform(GLchar const *name, glm::mat4 const &matrix) {
    glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_FALSE, glm::value_ptr(matrix));
}








