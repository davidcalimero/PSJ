#version 150

// In
in vec3 in_Position;
in vec4 in_Color;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec4 in_Tangent;

// Out
out vec3 ex_Position;
out vec4 ex_Color;
out vec3 ex_Normal;
out vec2 ex_TexCoord;
out vec4 ex_tangent;

// Matrix
uniform mat4 ModelMatrix;
uniform mat3 NormalMatrix;
layout(std140) uniform SharedMatrices
{
	mat4 ViewMatrix;
	mat4 ProjectionMatrix;
};

void main(void)
{
	ex_Color = in_Color;
	ex_Normal = normalize(NormalMatrix * in_Normal);
	ex_Position = normalize(vec3(ViewMatrix * ModelMatrix * vec4(in_Position, 1.0)));
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position, 1.0);


	// Sphere Mapping
	vec3 E = normalize(ex_Position);
	vec3 N = normalize(ex_Normal);
	vec3 R = reflect(E, N);

	float p = 2.0 * sqrt(2 * (R.z + 1.0)); 
	vec2 coords = vec2((R.x/p) + 0.5, (R.y/p) + 0.5);

	ex_TexCoord = coords;

	ex_tangent = in_Tangent;
}