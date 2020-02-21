#version 420

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

//uniform sampler2D heightMap;

layout (binding = 3) uniform sampler2D heightMap;

in vec3 in_vert;
in vec2 in_uv;
in vec3 in_normal;

out vec2 texcoord;
out vec3 norm;
out vec3 pos;
out vec3 texpos;

uniform float uTime;

void main()
{
	texcoord = in_uv;
	texcoord.y = 1 - texcoord.y;
	
	//norm = mat3(uView) * mat3(uModel) * in_normal;	

	//pos = (uView * uModel * vec4(in_vert, 1.0f)).xyz;
	
	//pos += normalize(norm) * 0.3 * sin(uTime * 10 + texcoord.y * 30);

	//gl_Position = uProj * vec4(pos, 1.0f);
	
	vec3 _vert = in_vert;
	_vert.y = texture(heightMap,in_uv).r*0.1f;
	gl_Position = uProj * uView * uModel * vec4(_vert,1.0);
	//texcoord = in_uv;
}
