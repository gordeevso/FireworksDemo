#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.hpp"


struct FreeTypeCharacter {
    GLuint mId;          // GL texture id
    glm::ivec2 mSize;    // Size of glyph
    glm::ivec2 mBearing; // Offset from baseline to left/top of glyph
    FT_Pos mAdvance;     // Horizontal offset to advance to next glyph
};


class TextRenderer
{
public:
    TextRenderer();
    ~TextRenderer();
    TextRenderer(TextRenderer const &) = default;
    TextRenderer &operator=(TextRenderer const &) = default;

    void Init(std::string font, GLuint fontSize);

    void RenderText(std::string text,
                  GLfloat x,
                  GLfloat y,
                  GLfloat scale,
                  glm::vec3 const & color = glm::vec3(1.0f));

private:
    std::unordered_map<GLchar, FreeTypeCharacter> mCharactersMap;
    std::shared_ptr<Shader> mShader;
    GLuint mVAO;
    GLuint mVBO;
};
