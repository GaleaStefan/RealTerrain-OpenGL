#version 330 core
in vec3 ex_color;
out vec4 out_color;

void main()
{
	out_color = vec4(ex_color.rgb, 1.0);
}