#version 450 core

in vec4 color_;
in vec2 text_coord_;
in vec3 normal_;
in vec3 world_position_;
out vec4 out_color_;

uniform sampler2D texture_sampler0_;
uniform vec3 view_position_;
uniform vec3 light_position_;

vec4 get_light_intensity(float square_size, float light_height)
{
	vec3 max_square = vec3(light_position_ + (square_size / 2));
	vec3 min_square = vec3(light_position_ - (square_size / 2));

	float x_intensity = 0.0f; 
	if(world_position_.x > light_position_.x)
	{
		x_intensity = (1.0f - ((world_position_.x - min_square.x) / (max_square.x - min_square.x)));
	}
	else
	{
		x_intensity = ((world_position_.x - min_square.x) / (max_square.x - min_square.x));
	}

	float z_intensity = 0.0f; 
	if(world_position_.z > light_position_.z)
	{
		z_intensity = (1.0f - ((world_position_.z - min_square.z) / (max_square.z - min_square.z)));
	}
	else
	{
		z_intensity = ((world_position_.z - min_square.z) / (max_square.z - min_square.z));
	}

	float final_intensity = (x_intensity + z_intensity) / 2.0f;
	if(world_position_.y > light_position_.y + light_height)
	{
		final_intensity = 0.0f;
	}
	return vec4(vec3(final_intensity), 1.0f);
}

void main()
{
	float ambient_strength = 0.1f;
	vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
	vec3 ambient = ambient_strength * light_color;
	vec3 normalized_normal = normalize(normal_);

	//diffuse
	vec4 light_intensity = get_light_intensity(12.0f, 3.0f);
	vec3 light_direction = normalize(light_position_ - world_position_);
	float diff = max(dot(normalized_normal, light_direction), 0.0f);
	vec3 diffuse = diff * light_color;

	//specular
	float specular_strength = 0.5f;
	int shininess = 32;
	vec3 view_direction = normalize(view_position_ - world_position_);
	vec3 reflect_direction = reflect(-light_direction, normalized_normal); //reflect attend un vecteur qui parte de la source de lumière vers le fragment, d'où la soustraction
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), shininess);
	vec3 specular = specular_strength * spec * light_color;

	vec4 result = vec4(ambient + diffuse + specular, 1.0f) * color_;

	//out_color_ = texture(texture_sampler0_, text_coord_);
	//out_color_ = color_;
	out_color_ = result + light_intensity;
}