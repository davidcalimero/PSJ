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
	// Cube Mapping 1 image
	vec3 E = normalize(ex_Position);
	vec3 N = normalize(ex_Normal);
	//vec3 reflected_vector = N;
	vec3 reflected_vector = normalize(reflect(E, N));

	float horizontal = 0.0f;
	float vertical = 0.0f;

	//float maxValue = max(max(abs(reflected_vector.x), abs(reflected_vector.y)), abs(reflected_vector.z)));
	float maxValue = 0.0f;
	if(maxValue == abs(reflected_vector.x))
	{
		// Significa que o vector reflectido vai apontar para as faces laterais
		if(reflected_vector.x < 0)
		{
			// Significa que o vector reflectido vai apontar para a face da esquerda.
			// A face da esquerda tem valores entre os 0.0 e 0.25 na horizontal, e 0.33 e 0.66 na vertical.
			// Antes de mais, devemos fazer com que os valores sejam convertidos para um intervalo entre 0 e 1 (de momento estão entre -1 e 1)
			horizontal = reflected_vector.z * 0.5 + 0.5;
			vertical = reflected_vector.y * 0.5 + 0.5;

			// Depois fazemos uma conversão, onde os valores de horizontal e vertical (de 0 a 1), têm de ser transformados para coordenadas nos intevalos acima descritos.
			horizontal = abs(1 - horizontal) * 0.25;
			vertical = (vertical * 0.33) + 0.33;

			// E com estes valores, temos as coordenadas para a textura.
		}
		else
		{
			// Significa que o vector reflectido vai apontar para a face da direita.
			// A face da direita tem valores entre os 0.50 e 0.75 na horizontal, e 0.33 e 0.66 na vertical.
			// Antes de mais, devemos fazer com que os valores sejam convertidos para um intervalo entre 0 e 1 (de momento estão entre -1 e 1)
			horizontal = reflected_vector.z * 0.5 + 0.5;
			vertical = reflected_vector.y * 0.5 + 0.5;

			// Depois fazemos uma conversão, onde os valores de horizontal e vertical (de 0 a 1), têm de ser transformados para coordenadas nos intevalos acima descritos.
			horizontal = (horizontal * 0.25) + 0.5;
			vertical = (vertical * 0.33) + 0.33;

			// E com estes valores, temos as coordenadas para a textura.
		}
	}
	else if(maxValue == abs(reflected_vector.y))
	{
		// Significa que o vector reflectido vai apontar para as faces verticais
		if(reflected_vector.y < 0)
		{
			// Significa que o vector reflectido vai apontar para a face de baixo.
			// A face da baixo tem valores entre os 0.25 e 0.50 na horizontal, e 0 e 0.33 na vertical.
			// Antes de mais, devemos fazer com que os valores sejam convertidos para um intervalo entre 0 e 1 (de momento estão entre -1 e 1)
			horizontal = reflected_vector.x * 0.5 + 0.5;
			vertical = reflected_vector.z * 0.5 + 0.5;

			// Depois fazemos uma conversão, onde os valores de horizontal e vertical (de 0 a 1), têm de ser transformados para coordenadas nos intevalos acima descritos.
			horizontal = (horizontal * 0.25) + 0.25;
			vertical = abs(1 - vertical) * 0.33;
		}
		else
		{
			// Significa que o vector reflectido vai apontar para a face de cima.
			// A face da direita tem valores entre os 0.25 e 0.50 na horizontal, e 0.66 e 1 na vertical.
			// Antes de mais, devemos fazer com que os valores sejam convertidos para um intervalo entre 0 e 1 (de momento estão entre -1 e 1)
			horizontal = reflected_vector.x * 0.5 + 0.5;
			vertical = reflected_vector.z * 0.5 + 0.5;

			// Depois fazemos uma conversão, onde os valores de horizontal e vertical (de 0 a 1), têm de ser transformados para coordenadas nos intevalos acima descritos.
			// No caso do vertical, nada é preciso ser feito, contudo com o horizontal, é preciso inverter.
			horizontal = (horizontal * 0.25) + 0.25;
			vertical = (vertical * 0.33) + 0.66;
		}
	}
	else 
	{
		// Significa que o vector reflectido vai apontar para as faces de frente e trás
		if(reflected_vector.z < 0)
		{
			// Significa que o vector reflectido vai apontar para a frente
			// A face da frente tem valores entre os 0.25 e 0.50 na horizontal, e 0.33 e 0.66 na vertical.
			// Antes de mais, devemos fazer com que os valores sejam convertidos para um intervalo entre 0 e 1 (de momento estão entre -1 e 1)
			horizontal = reflected_vector.x * 0.5 + 0.5;
			vertical = reflected_vector.y * 0.5 + 0.5;

			// Depois fazemos uma conversão, onde os valores de horizontal e vertical (de 0 a 1), têm de ser transformados para coordenadas nos intevalos acima descritos.
			horizontal = (horizontal * 0.25) + 0.25;
			vertical = (vertical * 0.33) + 0.33;
		}
		else
		{
			// Significa que o vector reflectido vai apontar para trás (na nossa direcção)
			// A face de trás tem valores entre os 0.75 e 1 na horizontal, e 0.33 e 0.66 na vertical.
			// Antes de mais, devemos fazer com que os valores sejam convertidos para um intervalo entre 0 e 1 (de momento estão entre -1 e 1)
			horizontal = reflected_vector.x * 0.5 + 0.5;
			vertical = reflected_vector.y * 0.5 + 0.5;

			// Depois fazemos uma conversão, onde os valores de horizontal e vertical (de 0 a 1), têm de ser transformados para coordenadas nos intevalos acima descritos.
			horizontal = (abs(1 - horizontal) * 0.25) + 0.75;
			vertical = (vertical * 0.33) + 0.33;
		}
	}

	// Aqui, com os valores calculados acima, vamos buscar a cor na posição calculada anteriormente
	out_Color = texture(baseTexture, vec2(horizontal,vertical));

	// Esta versão não tem a luz implementada!
}
