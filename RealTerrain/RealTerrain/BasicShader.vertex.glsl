#version 330 core
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_color;
layout(location = 3) in vec2 in_texture;
out vec3 ex_color;

void main()
{
	gl_Position = vec4(in_position.xyz, 1.0);
	ex_color = in_color;
}