#version 150

// In
in vec3 in_Position;
in vec4 in_Color;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec4 tangent;

// Out
out vec3 ex_Position;
out vec4 ex_Color;
out vec3 ex_Normal;
out vec2 ex_TexCoord;
out vec3 ex_L;
out vec3 ex_H;

//Light
uniform vec3 LightPosition;

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
	ex_Position = vec3(ViewMatrix * ModelMatrix * vec4(in_Position, 1.0));
	gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position, 1.0);
	ex_TexCoord = in_TexCoord;

	//Tangent

	vec3 N = ex_Normal;
	vec3 T = normalize(NormalMatrix * tangent.xyz);
	vec3 B = tangent.w * cross(N, T);

	vec3 v;

	vec3 lightDir = vec3(ViewMatrix * vec4(LightPosition, 1.0)) - ex_Position;
	v.x = dot(lightDir, T);
	v.y = dot(lightDir, B);
	v.z = dot(lightDir, N);
	ex_L = normalize(v);

	vec3 halfVector = normalize(lightDir + normalize(-ex_Position));
	v.x = dot(halfVector, T);
	v.y = dot(halfVector, B);
	v.z = dot(halfVector, N);
	ex_H = normalize(v);
}
