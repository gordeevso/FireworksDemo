#version 400 compatibility

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D gSampler;
uniform vec3 lightColor;

smooth in vec2 vTexCoord;
flat in vec4 vColorPart;

void main()
{
  vec4 vTexColor = texture2D(gSampler, vTexCoord);
  FragColor = vec4(vTexColor.xyz, 1.0) * vColorPart;

  float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
  if(brightness > 1.0)
  {
    BrightColor = vec4(FragColor.rgb, 1.0);
  }
}