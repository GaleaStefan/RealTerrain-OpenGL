#version 330 core
in vec3 f_position;
in vec3 f_normal;
in vec3 f_color;

uniform vec3 lightDirection;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
	vec3 norm = normalize(f_normal);
	
	vec3 ambient = 0.1 * lightColor;
	
	float diffuseFactor = max(dot(f_normal, -lightDirection), 0.0);
	vec3 diffuse = diffuseFactor * lightColor;

	vec3 result = (diffuse + ambient) * f_color;
	FragColor = vec4(result, 1.0);
}
