#version 330 core
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_color;
layout(location = 3) in vec2 v_texture;

out vec3 f_position;
out vec3 f_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(v_position, 1.0);
	f_position = vec3(model * vec4(v_position, 1.0));
	f_normal = mat3(transpose(inverse(model))) * v_normal;
}