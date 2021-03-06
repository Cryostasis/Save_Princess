#version 330 core

#define FRAG_OUTPUT0 0

uniform struct PointLight
{
	vec4 position;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec3 attenuation;
} light;

uniform struct Material
{
	sampler2D texture;

	vec4  ambient;
	vec4  diffuse;
	vec4  specular;
	vec4  emission;
	float shininess;
} material;

in Vertex 
{
	vec2  texcoord;
	vec3  normal;
	vec3  lightDir;
	vec3  viewDir;
	float distance;
} Vert;

layout(location = FRAG_OUTPUT0) out vec4 color;

void main(void)
{
	vec3 normal   = normalize(Vert.normal);
	vec3 lightDir = normalize(Vert.lightDir);
	vec3 viewDir  = normalize(Vert.viewDir);

	float attenuation = 1.0 / (light.attenuation[0] +
		light.attenuation[1] * Vert.distance +
		light.attenuation[2] * Vert.distance * Vert.distance);

	color = material.emission;

	color += material.ambient * light.ambient * attenuation;

	float NdotL = max(dot(normal, lightDir), 0.0);
	color += material.diffuse * light.diffuse * NdotL * attenuation;

	float RdotVpow = pow(max(dot(reflect(-viewDir, normal), lightDir), 0.0), material.shininess);
	color += material.specular * light.specular * RdotVpow * attenuation;

	//color *= vec4(0.5, 0.5, 0.5, 1.0) * 2;
	color *= texture(material.texture, Vert.texcoord);
}
~