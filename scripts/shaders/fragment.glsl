#version 150

// In
in vec3 ex_Position;
in vec4 ex_Color;
in vec3 ex_Normal;
in vec2 ex_TexCoord;
in vec3 ex_L;
in vec3 ex_H;

// Out
out vec4 out_Color;

// Light Attributes
uniform vec3 LightPosition;
uniform vec3 AmbientLight;
uniform vec3 DiffuseLight;
uniform vec3 SpecularLight;
uniform vec3 LightAttenuation;


// Material Attributes
uniform vec3 MaterialAmbient;
uniform vec3 MaterialDiffuse;
uniform vec3 MaterialSpecular;
uniform float MaterialShininess;



// Texture Sample
uniform sampler2D baseTexture;
uniform sampler2D normalTexture;

// Matrix
uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


void main(void)
{
	// Blinn-Phong Model
	// Vector Initialization
	vec3 lightDir = vec3(ViewMatrix * vec4(LightPosition, 1.0)) - ex_Position;
	float LightDistance = length(lightDir);
	vec3 N = 2.0 * texture(normalTexture, ex_TexCoord).rgb - 1.0;
	N = normalize(N);

	// Ambient Component
	vec3 ambient = MaterialAmbient * AmbientLight;

	// Diffuse Component
	float NdotL = max(dot(N, ex_L), 0.0);
	vec3 diffuse = MaterialDiffuse * DiffuseLight * NdotL;

	// Specular Component
	vec3 specular = vec3(0.0);
	if(NdotL > 0)
	{
		float NdotH = max(dot(N, ex_H), 0.0);
		specular = MaterialSpecular * SpecularLight * pow(NdotH, MaterialShininess * 128);
	}

	float attenuation = 1.0 / (LightAttenuation.x + LightAttenuation.y * LightDistance + LightAttenuation.z * LightDistance * LightDistance);
	
	vec4 LightIntensity = vec4(ambient + diffuse*attenuation + specular*attenuation, 1.0);
	vec4 textureColor = texture(baseTexture, ex_TexCoord);
	out_Color = LightIntensity * textureColor;
}
