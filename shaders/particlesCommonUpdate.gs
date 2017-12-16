#version 400 compatibility

layout(points) in;
layout(points) out;
layout(max_vertices = 40) out;

// All that we get from vertex shader

in vec2 vPositionPass[];
in vec2 vVelocityPass[];
in vec3 vColorPass[];
in float fLifeTimePass[];
in float fSizePass[];
in int iTypePass[];

// All that we send further

out vec2 vPositionOut;
out vec2 vVelocityOut;
out vec3 vColorOut;
out float fLifeTimeOut;
out float fSizeOut;
out int iTypeOut;

uniform vec2 mVec2EmitPosition; // Position where new particles are spawned
uniform vec2 mVec2EmitGravity; // Gravity vector for particles - updates velocity of particles
uniform vec2 mVec2EmitVelocityMin; // Velocity of new particle - from min to (min+range)
uniform vec2 mVec2EmitVelocityRange;

uniform vec3 mVec3EmitColor;
uniform float mFEmitSize;

uniform float mFEmitLifeTimeMin, mFEmitLifeTimeRange; // Life of new particle - from min to (min+range)
uniform float fTimePassed; // Time passed since last frame

uniform vec3 vRandomSeed; // Seed number for our random number function
vec3 vLocalSeed;

uniform int mIntNumTargetEmission; // How many particles will be generated next time, if greater than zero, particles are generated

// This function returns random number from zero to one
float randZeroOne()
{
    uint n = floatBitsToUint(vLocalSeed.y * 214013.0 + vLocalSeed.x * 2531011.0 + vLocalSeed.z * 141251.0);
    n = n * (n * n * 15731u + 789221u);
    n = (n >> 9u) | 0x3F800000u;
 
    float fRes =  2.0 - uintBitsToFloat(n);
    vLocalSeed = vec3(vLocalSeed.x + 147158.0 * fRes, vLocalSeed.y*fRes  + 415161.0 * fRes, vLocalSeed.z + 324154.0*fRes);
    return fRes;
}

void main()
{
  // gl_Position doesn't matter now, as rendering is discarded, so I don't set it at all

  vPositionOut = vPositionPass[0];
  vVelocityOut = vVelocityPass[0];
  if(iTypePass[0] != 0)
  {
    vPositionOut += vVelocityOut * fTimePassed;
    vVelocityOut += mVec2EmitGravity * fTimePassed;
  }

  vColorOut = vColorPass[0];
  fLifeTimeOut = fLifeTimePass[0] - fTimePassed;
  fSizeOut = fSizePass[0];
  iTypeOut = iTypePass[0];
    
  if(iTypeOut == 0)
  {
    EmitVertex();
    EndPrimitive();
    
    for(int i = 0; i < mIntNumTargetEmission; i++)
    {
      vPositionOut = mVec2EmitPosition;
      vVelocityOut = mVec2EmitVelocityMin + vec2(mVec2EmitVelocityRange.x * vRandomSeed.x, mVec2EmitVelocityRange.y * vRandomSeed.y);
      vColorOut = mVec3EmitColor;
      fLifeTimeOut = mFEmitLifeTimeMin + mFEmitLifeTimeRange * randZeroOne();
      fSizeOut = mFEmitSize;
      iTypeOut = 1;
      EmitVertex();
      EndPrimitive();
    }
  }
  else if(fLifeTimeOut > 0.0)
  {
      EmitVertex();
      EndPrimitive(); 
  }
}