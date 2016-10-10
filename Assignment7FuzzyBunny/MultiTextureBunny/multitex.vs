#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;

out VS_OUT {
    vec3 Normal;
} vs_out;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{

    Normal = normalize( NormalMatrix * VertexNormal);
	vs_out.Normal = normalize(vec3(ProjectionMatrix * vec4(Normal, 0.0)));
    Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );

    gl_Position = MVP * vec4(VertexPosition,1.0);
}
