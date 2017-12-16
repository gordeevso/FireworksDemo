#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>

#include "Shader.hpp"
#include "Texture.hpp"

std::string const EFFECTS_LIGHT_PROGRAM = "effectsLightProgram";
std::string const EFFECTS_BLUR_PROGRAM = "effectsBlurProgram";
std::string const EFFECTS_FINAL_PROGRAM = "effectsFinalProgram";
std::string const PARTICLE_COMMON_UPDATE_PROGRAM = "particleCommonUpdateProgram";
std::string const PARTICLE_COMMON_RENDER_PROGRAM = "particleCommonRenderProgram";
std::string const PARTICLE_STRAFE_UPDATE_PROGRAM = "particleStrafeUpdateProgram";
std::string const PARTICLE_STRAFE_RENDER_PROGRAM = "particleStrafeRenderProgram";
std::string const PARTICLE_EXPLODE_UPDATE_PROGRAM = "particleExplodeUpdateProgram";
std::string const PARTICLE_EXPLODE_STRAFE_UPDATE_PROGRAM = "particleExplodeStrafeUpdateProgram";
std::string const PARTICLE_EXPLODE_RENDER_PROGRAM = "particleExplodeRenderProgram";
std::string const PARTICLE_TEXTURE = "particleTexture";
std::string const PARTICLE_TEXTURE1 = "particleTexture1";
std::string const PARTICLE_TEXTURE2 = "particleTexture2";
std::string const PARTICLE_TEXTURE3 = "particleTexture3";
std::string const PARTICLE_TEXTURE4 = "particleTexture4";
std::string const PARTICLE_TEXTURE5 = "particleTexture5";
std::string const PARTICLE_TEXTURE6 = "particleTexture6";
std::string const PARTICLE_TEXTURE7 = "particleTexture7";
std::string const PARTICLE_TEXTURE8 = "particleTexture8";
std::string const PARTICLE_TEXTURE9 = "particleTexture9";
std::string const CLOUD1_TEXTURE = "cloud1Texture";
std::string const CLOUD2_TEXTURE = "cloud2Texture";
std::string const CLOUD3_TEXTURE = "cloud3Texture";
std::string const CLOUD4_TEXTURE = "cloud4Texture";
std::string const CLOUD5_TEXTURE = "cloud5Texture";
std::string const CLOUD6_TEXTURE = "cloud6Texture";
std::string const CLOUD7_TEXTURE = "cloud7Texture";
std::string const CLOUD8_TEXTURE = "cloud8Texture";
std::string const CLOUD9_TEXTURE = "cloud9Texture";
std::string const CLOUD10_TEXTURE = "cloud10Texture";
std::string const RENDER_TO_TEXTURE_PROGRAM = "renderToTexture";
std::string const SPRITE_PROGRAM = "spriteShader";


typedef std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderMap;
typedef std::unordered_map<std::string, std::shared_ptr<Texture>> TextureMap;

class ResourceManager {
public:
    static void LoadResources();
    static void LoadShader(std::string const &program_name,
                           std::string const &vs_file_path,
                           std::string const &fs_file_path,
                           std::string const &gs_file_path = "");
    static void LoadTexture(std::string const &texture_path,
                               GLboolean alpha,
                               std::string const &name);

    static std::shared_ptr<Shader> GetShader(std::string const &name);
    static std::shared_ptr<Texture> GetTexture(std::string const &name);
    static std::vector<std::string> const & GetTextureNames();

private:
    ResourceManager() = delete;
    ResourceManager(ResourceManager const &) = delete;
    ResourceManager &operator=(ResourceManager const &) = delete;

private:
    static ShaderMap mShaders;
    static TextureMap mTextures;
    static std::vector<std::string> mTextureNames;
};




