#version 420

uniform vec3 uSceneAmbient = vec3(0.7, 0.7, 0.7);
uniform vec3 uLightPosition = vec3(3.0, 0.0, 0.0);
uniform vec3 uLightColor = vec3(1.0, 0.9, 0.8);
uniform float uLightSpecularExponent = 16.0;
layout(binding = 0) uniform sampler2D uTexAlbedo;
layout(binding = 1) uniform sampler2D uTexEmissive;
layout(binding = 2) uniform sampler2D uTexSpecular;
layout(binding = 4) uniform sampler2D uTexRock;
layout(binding = 5) uniform sampler2D uTexGrass;
layout(binding = 6) uniform sampler2D uTexSand;
layout(binding = 7) uniform sampler2D uTexWater;

uniform float uTime;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;

out vec4 outColor;

void main()
{
    outColor = texture2D(uTexWater,texcoord);
	outColor.a = 0.9;
}
