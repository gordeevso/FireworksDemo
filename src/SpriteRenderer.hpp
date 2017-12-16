#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.hpp"
#include "Texture.hpp"

class SpriteRenderer
{
public:
    SpriteRenderer();
    SpriteRenderer(SpriteRenderer const &) = delete;
    SpriteRenderer & operator=(SpriteRenderer const &) = delete;
    ~SpriteRenderer();

    void DrawSprite(std::shared_ptr<Texture> texture,
                    glm::vec2 const & position,
                    glm::vec2 const & size = glm::vec2(1.f, 1.f),
                    GLfloat rotate_degrees = 0.f,
                    glm::vec3 const & color = glm::vec3(1.f, 1.f, 1.f));

private:
    std::shared_ptr<Shader> mShader;
    GLuint mVAO;
    GLuint mVBO;

private:
    void InitSpriteRenderData();
};


