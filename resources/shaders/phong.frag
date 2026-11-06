#version 450 core

in vec4 color_;
in vec2 texcoord_;
in vec3 normal_;
in vec3 world_position_;
out vec4 out_color_;

//uniform sampler2D texture_sampler0_;

//TODO : hardcodé
uniform vec3 view_position_;
uniform vec3 light_position_;
uniform vec3 light_position2_;
uniform vec3 light_position3_;

vec4 point_light(vec3 light_position)
{
	float distance = length(light_position - world_position_);
	float attenuation = 1.0 / (1.0 + 0.07 * distance + 0.017 * (distance * distance));
	vec3 normalized_normal = normalize(normal_);
	vec3 light_color = vec3(1.0);

	//ambient
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;
	ambient *= attenuation;

	//diffuse
	vec3 light_direction = normalize(light_position - world_position_);
	float diff = max(dot(normalized_normal, light_direction), 0.0);
	vec3 diffuse = diff * light_color;
	diffuse *= attenuation;

	//specular
	float specular_strength = 0.5;
	int shininess = 32;
	vec3 view_direction = normalize(view_position_ - world_position_);
	vec3 reflect_direction = reflect(-light_direction, normalized_normal); //reflect attend un vecteur qui parte de la source de lumière vers le fragment, d'où la soustraction
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0), shininess);
	vec3 specular = specular_strength * spec * light_color;
	specular *= attenuation;

	return vec4(ambient + diffuse + specular, 1.0);
}

void main()
{
	vec4 result = point_light(light_position_);
	result += point_light(light_position2_);
	result += point_light(light_position3_);

	//out_color_ = texture(texture_sampler0_, texcoord_);
	//out_color_ = color_;
	out_color_ = result * color_;
}