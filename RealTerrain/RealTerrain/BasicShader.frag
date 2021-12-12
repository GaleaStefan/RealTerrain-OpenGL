#version 330 core
in vec3 f_position;
in vec3 f_normal;
//layout (location = 2) in vec3 f_color;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec3 objectColor;

out vec4 FragColor;

void main()
{
	//out_color = vec4(ex_color.rgb, 1.0);
	vec3 norm = normalize(f_normal);
	vec3 lightDirection = normalize(lightPosition - f_position);
	
	vec3 ambient = 0.1 * lightColor;
	
	float diffuseFactor = max(dot(f_normal, lightDirection), 0.0);
	vec3 diffuse = diffuseFactor * lightColor;

	vec3 result = (diffuse + ambient) * objectColor;
	FragColor = vec4(result, 1.0);
}