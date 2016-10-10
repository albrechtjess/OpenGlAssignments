#version 400
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

in VS_OUT{
	vec3 Normal;
}gs_in[];
const float MAGNITUDE = 0.1f;

void main() {  
    
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

	gl_Position = gl_in[0].gl_Position + vec4(gs_in[0].Normal, 0.0) * MAGNITUDE;
    EmitVertex();

    EndPrimitive();
} 
