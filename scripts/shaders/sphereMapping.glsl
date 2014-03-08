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
uniform sampler2D tex1;
uniform sampler2D tex2;

// Matrix
uniform mat4 ModelMatrix;
layout(std140) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};


void main(void)
{
	// Sphere Mapping
	

	// Esta versão não tem a luz implementada!
}
