#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "GLFWWrapper.hpp"
#include "ResourceManager.hpp"
#include "TextRenderer.hpp"

std::string const TEXT_SHADER = "text_shader";

TextRenderer::TextRenderer() : mCharactersMap{},
                               mShader{},
                               mVAO{},
                               mVBO{} {
    Init("../res/fonts/BKANT.TTF", 48);
}

void TextRenderer::Init(std::string font, GLuint fontSize)
{

    ResourceManager::LoadShader(TEXT_SHADER,
                                "../shaders/text.vs",
                                "../shaders/text.fs");

    mShader = ResourceManager::GetShader(TEXT_SHADER);
    mShader->LinkProgram();
    mShader->Use();

    glm::mat4 projection = glm::ortho(0.0f,
                                      static_cast<GLfloat>(GLFWWrapper::GetInstance()->GetWidth()),
                                      static_cast<GLfloat>(GLFWWrapper::GetInstance()->GetHeight()),
                                      0.0f);

    mShader->SetUniform("text", 0);
    mShader->SetUniform("projection", projection);

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mCharactersMap.clear();

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cerr << "ERROR::FREETYPE: Could not init FreeType Library\n";

    FT_Face face;
    if (FT_New_Face(ft, font.c_str(), 0, &face))
        std::cerr << "ERROR::FREETYPE: Failed to load font\n";

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "ERROR::FREETYTPE: Failed to load Glyph\n";
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        FreeTypeCharacter character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };

        mCharactersMap.emplace(std::make_pair(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text,
                              GLfloat x,
                              GLfloat y,
                              GLfloat scale,
                              glm::vec3 const & color)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mShader->Use();
    mShader->SetUniform("textColor", color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(mVAO);

    for (auto c = text.begin(); c != text.end(); c++)
    {
        FreeTypeCharacter ch = mCharactersMap[*c];

        GLfloat posX = x + ch.mBearing.x * scale;
        GLfloat posY = y + (mCharactersMap['H'].mBearing.y - ch.mBearing.y) * scale;

        GLfloat w = ch.mSize.x * scale;
        GLfloat h = ch.mSize.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
                { posX,     posY + h,   0.f, 1.f },
                { posX + w, posY,       1.f, 0.f },
                { posX,     posY,       0.f, 0.f },

                { posX,     posY + h,   0.f, 1.f },
                { posX + w, posY + h,   1.f, 1.f },
                { posX + w, posY,       1.f, 0.f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.mId);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph
        x += (ch.mAdvance >> 6) * scale; // Bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_CULL_FACE);

}

TextRenderer::~TextRenderer() {
    glDeleteBuffers(1, &mVBO);
    glDeleteVertexArrays(1, &mVAO);

    for (auto c = mCharactersMap.begin(); c != mCharactersMap.end(); c++) {
        glDeleteTextures(1, &((*c).second.mId));
    }
}
