#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;

uniform vec2 offset;

void main() {
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gl_Position = gl_in[0].gl_Position - vec4(offset.x, offset.y, 0.0, 0.0);
    EmitVertex();


    EndPrimitive();
}