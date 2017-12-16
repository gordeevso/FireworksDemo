#version 400 compatibility

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vVelocity;
layout (location = 2) in vec3 vColor;
layout (location = 3) in float fLifeTime;
layout (location = 4) in float fSize;
layout (location = 5) in int iType;

out vec2 vPositionPass;
out vec2 vVelocityPass;
out vec3 vColorPass;
out float fLifeTimePass;
out float fSizePass;
out int iTypePass;

void main()
{
  vPositionPass = vPosition;
  vVelocityPass = vVelocity;
  vColorPass = vColor;
  fLifeTimePass = fLifeTime;
  fSizePass = fSize;
  iTypePass = iType;
}