#version 330

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

layout(location = 0) in vec3 in_vert;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;

out vec2 texcoord;
out vec3 norm;
out vec3 pos;

uniform float uTime;

void main()
{
	texcoord = in_uv;
	texcoord.y = 1 - texcoord.y;
	
	norm = mat3(uView) * mat3(uModel) * in_normal;

	pos = (uView * uModel * vec4(in_vert, 1.0f)).xyz;
	
	pos += normalize(norm) * 0.2 * sin(uTime * 10 + texcoord.y * 30);

	gl_Position = uProj * vec4(pos, 1.0f);
}
