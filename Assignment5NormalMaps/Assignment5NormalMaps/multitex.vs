#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;
layout (location = 3) in vec4 VertexTangent;

struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 Intensity; // A,D,S intensity
};
uniform LightInfo Light;

out vec3 Position;
out vec3 Normal;
out vec3 Binormal;
out vec3 Tangent;
out vec2 TexCoord_2;

out vec3 light_direction;
out vec3 view_direction;

uniform float time;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
	TexCoord_2 = VertexTexCoord;

	//Transform normal and tangent to eye space
    Normal = normalize( NormalMatrix * VertexNormal);
	Tangent = normalize( NormalMatrix * vec3(VertexTangent));

    Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );

	Binormal = normalize(cross(Normal, Tangent))*VertexTangent.w; //compute Binormal
	
	//matrix for transformation to tangent space
	mat3 tangentSpace = mat3(Tangent.x, Binormal.x, Normal.x,
							 Tangent.y, Binormal.y, Normal.y,
							 Tangent.z, Binormal.z, Normal.z);
	
	light_direction = normalize(vec3(Light.Position) - Position); //light direction
    view_direction = normalize(-Position.xyz); //view direction

	light_direction *= tangentSpace;
	view_direction *= tangentSpace;

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
