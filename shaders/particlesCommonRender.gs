#version 400 compatibility

uniform mat4 umProjection;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in vec3 vColorPass[];
in float fLifeTimePass[];
in float fSizePass[];
in int iTypePass[];

smooth out vec2 vTexCoord;
flat out vec4 vColorPart;

void main()
{
  if(iTypePass[0] != 0)
  {
    vec2 vPosOld = gl_in[0].gl_Position.xy;
    float fSize = fSizePass[0];

    vColorPart = vec4(vColorPass[0], fLifeTimePass[0]);

    vec2 vecQuadY = vec2(0.0, 1.0); //ortho basis
    vec2 vecQuadX = vec2(1.0, 0.0);

    vec2 vPos = vPosOld + (-vecQuadX - vecQuadY) * fSize;
    vTexCoord = vec2(0.0, 0.0);
    gl_Position = umProjection*vec4(vPos, 0.0, 1.0);
    EmitVertex();
    
    vPos = vPosOld + (-vecQuadX + vecQuadY) * fSize;
    vTexCoord = vec2(0.0, 1.0);
    gl_Position = umProjection * vec4(vPos, 0.0, 1.0);
    EmitVertex();
    
    vPos = vPosOld + (vecQuadX - vecQuadY) * fSize;
    vTexCoord = vec2(1.0, 0.0);
    gl_Position = umProjection * vec4(vPos, 0.0, 1.0);
    EmitVertex();
    
    vPos = vPosOld + (vecQuadX + vecQuadY) * fSize;
    vTexCoord = vec2(1.0, 1.0);
    gl_Position = umProjection * vec4(vPos, 0.0, 1.0);
    EmitVertex();
      
    EndPrimitive();
  }
}