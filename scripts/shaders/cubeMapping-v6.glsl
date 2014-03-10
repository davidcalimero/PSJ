#version 150

// In
in vec3 ex_Position;
in vec4 ex_Color;
in vec3 ex_Normal;
in vec2 ex_TexCoord;

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
uniform samplerCube cubeMap;
//uniform sampler2D baseTexture;
uniform sampler2D normalTexture;

// Matrix
uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices
{	
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

const float reflect_factor = 0.5;

void main (void) {

	vec3 E = normalize(ex_Position);
	vec3 N = normalize(ex_Normal);
	//vec3 reflected_vector = N;
	vec3 reflected_vector = normalize(reflect(-E, N));

	// Perform a simple 2D texture look up.
	//vec3 base_color = texture(baseTexture, ex_TexCoord).rgb;

	// Perform a cube map look up.
	vec3 cube_color = textureCube(cubeMap, reflected_vector).rgb;

	// Write the final pixel.
	out_Color = vec4(cube_color, 1.0);
	//out_Color = vec4(mix(base_color, cube_color, reflect_factor), 1.0);
}