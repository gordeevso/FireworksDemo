#include <fstream>
#include <sstream>
#include <iostream>
#include <SOIL.h>

#include <glad/glad.h>
#include "ResourceManager.hpp"

TextureMap ResourceManager::mTextures;
ShaderMap ResourceManager::mShaders;
std::vector<std::string> ResourceManager::mTextureNames;

//Shader-specific functions
std::shared_ptr<Shader> ResourceManager::GetShader(std::string const &name) {
    return mShaders[name];
}

void ResourceManager::LoadShader(std::string const &program_name,
                                 std::string const &vs_file_path,
                                 std::string const &fs_file_path,
                                 std::string const &gs_file_path)
{
    std::string vs_source;
    std::string fs_source;
    std::string gs_source;

    std::ifstream handle_vs_file(vs_file_path);
    if(handle_vs_file.is_open()) {
        std::stringstream ss_vs;
        ss_vs << handle_vs_file.rdbuf();
        vs_source = ss_vs.str();
    }
    else {
        throw std::invalid_argument(vs_file_path);
    }

    std::ifstream handle_fs_file(fs_file_path);
    if(handle_fs_file.is_open()) {
        std::stringstream ss_fs;
        ss_fs << handle_fs_file.rdbuf();
        fs_source = ss_fs.str();
    }
    else {
        throw std::invalid_argument(fs_file_path);
    }

    if(!gs_file_path.empty())
    {
        std::ifstream handle_gs_file(gs_file_path);
        if(handle_gs_file.is_open()) {
            std::stringstream ss_gs;
            ss_gs << handle_gs_file.rdbuf();
            gs_source = ss_gs.str();
        }
        else {
            throw std::invalid_argument(fs_file_path);
        }
    }

    mShaders[program_name] = std::make_shared<Shader>();
    mShaders[program_name]->AttachShaders(vs_source, fs_source, gs_source);
}



//Texture-specific functions
std::shared_ptr<Texture> ResourceManager::GetTexture(std::string const &name) {
    return mTextures[name];
}

void ResourceManager::LoadTexture(std::string const &texture_file,
                                  GLboolean alpha,
                                  std::string const &name) {
    mTextures[name] = std::make_shared<Texture>();
    mTextureNames.push_back(name);

    if (alpha) {
        mTextures[name]->SetImageFormat(GL_RGBA);
        mTextures[name]->SetInternalFormat(GL_RGBA);
    }

    int32_t width{};
    int32_t height{};
    uint8_t *image = SOIL_load_image(texture_file.c_str(),
                                     &width,
                                     &height,
                                     0,
                                     mTextures[name]->GetImageFormat() == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);

    mTextures[name]->Generate(width, height, image);

    SOIL_free_image_data(image);
}

std::vector<std::string> const & ResourceManager::GetTextureNames() {
    return mTextureNames;
}

void ResourceManager::LoadResources()
{
    ResourceManager::LoadShader(RENDER_TO_TEXTURE_PROGRAM,
                                "../shaders/renderToTexture.vs",
                                "../shaders/renderToTexture.fs");
    ResourceManager::LoadShader(SPRITE_PROGRAM,
                                "../shaders/sprite.vs",
                                "../shaders/sprite.fs");
    ResourceManager::LoadTexture("../res/textures/particle1.png", GL_TRUE, PARTICLE_TEXTURE1);
    ResourceManager::LoadTexture("../res/textures/particle2.png", GL_TRUE, PARTICLE_TEXTURE2);
    ResourceManager::LoadTexture("../res/textures/particle3.png", GL_TRUE, PARTICLE_TEXTURE3);
    ResourceManager::LoadTexture("../res/textures/particle4.png", GL_TRUE, PARTICLE_TEXTURE4);
    ResourceManager::LoadTexture("../res/textures/particle5.png", GL_TRUE, PARTICLE_TEXTURE5);
    ResourceManager::LoadTexture("../res/textures/particle6.png", GL_TRUE, PARTICLE_TEXTURE6);
    ResourceManager::LoadTexture("../res/textures/particle7.png", GL_TRUE, PARTICLE_TEXTURE7);
    ResourceManager::LoadTexture("../res/textures/particle8.png", GL_TRUE, PARTICLE_TEXTURE8);
    ResourceManager::LoadTexture("../res/textures/particle9.png", GL_TRUE, PARTICLE_TEXTURE9);
    ResourceManager::LoadTexture("../res/textures/cloud1.png", GL_TRUE, CLOUD1_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud2.png", GL_TRUE, CLOUD2_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud3.png", GL_TRUE, CLOUD3_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud4.png", GL_TRUE, CLOUD4_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud5.png", GL_TRUE, CLOUD5_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud6.png", GL_TRUE, CLOUD6_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud7.png", GL_TRUE, CLOUD7_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud8.png", GL_TRUE, CLOUD8_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud9.png", GL_TRUE, CLOUD9_TEXTURE);
    ResourceManager::LoadTexture("../res/textures/cloud10.png", GL_TRUE, CLOUD10_TEXTURE);
//    ResourceManager::LoadShader(PARTICLE_COMMON_UPDATE_PROGRAM,
//                                "../shaders/particlesCommonUpdate.vs",
//                                "../shaders/particlesCommonUpdate.fs",
//                                "../shaders/particlesCommonUpdate.gs");
//    ResourceManager::LoadShader(PARTICLE_COMMON_RENDER_PROGRAM,
//                                "../shaders/particlesCommonRender.vs",
//                                "../shaders/particlesCommonRender.fs",
//                                "../shaders/particlesCommonRender.gs");
    ResourceManager::LoadShader(PARTICLE_STRAFE_UPDATE_PROGRAM,
                                "../shaders/particlesCommonUpdate.vs",
                                "../shaders/particlesCommonUpdate.fs",
                                "../shaders/particlesStrafeUpdate.gs");
    auto program = ResourceManager::GetShader(PARTICLE_STRAFE_UPDATE_PROGRAM);
    const char *sVaryings[6] =
            {
                    "vPositionOut",
                    "vVelocityOut",
                    "vColorOut",
                    "fLifeTimeOut",
                    "fSizeOut",
                    "iTypeOut",
            };
    for (int32_t i = 0; i != 6; ++i) {
        glTransformFeedbackVaryings(program->GetId(), 6, sVaryings, GL_INTERLEAVED_ATTRIBS);
    }

    ResourceManager::LoadShader(PARTICLE_STRAFE_RENDER_PROGRAM,
                                "../shaders/particlesCommonRender.vs",
                                "../shaders/effectsLight.fs",
                                "../shaders/particlesCommonRender.gs");
    ResourceManager::LoadShader(PARTICLE_EXPLODE_UPDATE_PROGRAM,
                                "../shaders/particlesCommonUpdate.vs",
                                "../shaders/particlesCommonUpdate.fs",
                                "../shaders/particlesExplodeUpdate.gs");
    program = ResourceManager::GetShader(PARTICLE_EXPLODE_UPDATE_PROGRAM);

    for (int32_t i = 0; i != 6; ++i) {
        glTransformFeedbackVaryings(program->GetId(), 6, sVaryings, GL_INTERLEAVED_ATTRIBS);
    }

    ResourceManager::LoadShader(PARTICLE_EXPLODE_RENDER_PROGRAM,
                                "../shaders/particlesCommonRender.vs",
                                "../shaders/effectsLight.fs",
                                "../shaders/particlesCommonRender.gs");
    ResourceManager::LoadShader(EFFECTS_BLUR_PROGRAM,
                                "../shaders/effectsBlur.vs",
                                "../shaders/effectsBlur.fs");
    ResourceManager::LoadShader(EFFECTS_FINAL_PROGRAM,
                                "../shaders/effectsFinal.vs",
                                "../shaders/effectsFinal.fs");
}





