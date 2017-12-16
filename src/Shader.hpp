#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>


class Shader
{
public:
    explicit Shader() = default;

    ~Shader();

    void Use() const;

    GLuint CreateShader(const GLenum shaderType,
                        const std::string &shaderSource) const;

    void AttachShaders(std::string const &vertexShaderCode,
                       std::string const &fragmentShaderCode,
                       std::string const &geometryShaderCode = "");
    void LinkProgram();

    void DeleteProgram(); 
    GLuint GetId() const;

    void SetUniform(GLchar const *name, GLfloat value);
    void SetUniform(GLchar const *name, GLint value);
    void SetUniform(GLchar const *name, GLfloat x, GLfloat y);
    void SetUniform(GLchar const *name, glm::vec2 const &value);
    void SetUniform(GLchar const *name, GLfloat x, GLfloat y, GLfloat z);
    void SetUniform(GLchar const *name, glm::vec3 const &value);
    void SetUniform(GLchar const *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    void SetUniform(GLchar const *name, glm::vec4 const &value);
    void SetUniform(GLchar const *name, glm::mat4 const &matrix);

private:
    GLuint mId;
    std::vector<GLuint> mShaderIds;
    bool mLinked;
};





