#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 7) out;

in VS_OUT {
    vec3 color;
} gs_in[];

out vec3 fColor;

void hexagon(vec4 position)
{
	fColor = gs_in[0].color;
	gl_Position = position + vec4(-0.1, -0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4( 0.1, -0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4( 0.2,  0.0, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4( 0.1, 0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-0.1, 0.1, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-0.2, 0.0, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4(-0.1, -0.1, 0.0, 0.0);
	EmitVertex();
	
	EndPrimitive();
}

void build_house(vec4 position)
{    
    fColor = gs_in[0].color; // gs_in[0] since there's only one input vertex
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0); // 1:bottom-left   
    EmitVertex();   
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
    EmitVertex();
    gl_Position = position + vec4(-0.05,  0.45, 0.0, 0.0); // 5:top 1
    fColor = vec3(0.8, 1.0, 0.9);
    EmitVertex();
    gl_Position = position + vec4( 0.05, 0.45, 0.0, 0.0); // 6:top 2
    fColor = vec3(1.0, 0.7, 0.8);
    EmitVertex();
    EndPrimitive();
}

void lines(vec4 position)
{
	fColor = gs_in[0].color;
	gl_Position = position + vec4(-0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	gl_Position = position + vec4( 0.1, 0.0, 0.0, 0.0);
	EmitVertex();
	
	EndPrimitive();
}

void points(vec4 position)
{
	fColor = gs_in[0].color;
	gl_Position = position;
	EmitVertex();
	EndPrimitive();
}

void main() {
    hexagon(gl_in[0].gl_Position);
}
