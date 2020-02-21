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

uniform float uTime;

in vec2 texcoord;
in vec3 norm;
in vec3 pos;
in vec3 texpos;

out vec4 outColor;

void main()
{
	if(pos.x > -0.0000f)
		outColor = texture2D(uTexRock,texcoord)*0.9;
	else
	{
		float delta = pos.y/5;
		outColor = texture2D(uTexGrass,texcoord)*delta +
				texture2D(uTexGrass,texcoord)*(1-delta);
	}

	//vec2 texOffset = texcoord;
	//texOffset *= 0.1;
	//texOffset.y -= uTime;
	//texOffset.x += sin(texcoord.y + uTime);
	//texOffset.y += sin(texcoord.y + uTime);
	//vec4 albedo = texture(uTexGrass,texOffset);

	//outColor.rgb = uSceneAmbient * albedo.rgb; 
	
	
	
	
	
	//outColor.a = albedo.a;

	// Fix length after rasterizer interpolates
	//vec3 normal = normalize(norm);

	//vec3 lightVec = uLightPosition - pos;
	//float dist = length(lightVec);
	//vec3 lightDir = lightVec / dist;

	//float NdotL = dot(normal, lightDir);

	// If the normal is facing the light
	//if (NdotL > 0.0)
	//{
		// Normalized vector pointing towards the camera
		//vec3 eye = normalize(-pos);
		
		// Calculate attenuation (falloff)
		// Add a small number to avoid divide by zero.
		//float attenuation = 1.0 / (1.0 + dist * dist * 0.1);

		// Calculate the diffuse contribution
		//outColor.rgb += albedo.rgb * uLightColor * NdotL * attenuation;
		
		//vec3 reflection = reflect(-lightDir, normal);
		
		//float specularStrength = dot(reflection, eye);
		//specularStrength = max(specularStrength, 0.0f); // don't let it fall before zero

		//vec3 specularColor = texture(uTexSpecular, texOffset).rgb;
		// Calculate the specular contribution
		//outColor.rgb += specularColor * uLightColor * pow(specularStrength, uLightSpecularExponent) * attenuation;
	//}

	//outColor.b = 0;
	//outColor.rg = texcoord.xy;
	//outColor += texture(uTexAlbedo,texOffset);
	//outColor += texture(uTexEmissive,texOffset);
	//outColor += texture(uTexSpecular,texcoord);
}
