#version 450 core

in vec4 output_color_;
in vec2 output_text_coord_;
in vec3 output_normal_;
in vec3 world_position_;
out vec4 out_color_;

uniform sampler2D texture_sampler0_;
uniform vec3 view_position_;

void main()
{
	/*float ambient_strength = 0.1f;
	vec3 light_color = vec3(1.0f, 1.0f, 1.0f);
	vec3 light_position = vec3(5.0f, 5.0f, 5.0f);
	vec3 ambient = ambient_strength * light_color;
	vec3 normalized_normal = normalize(output_normal_);
	vec3 light_direction = normalize(light_position - world_position_);
	float diff = max(dot(normalized_normal, light_direction), 0.0f);
	vec3 diffuse = diff * light_color;
	float specular_strength = 0.5f;
	vec3 view_direction = normalize(view_position_ - world_position_);
	vec3 reflect_direction = reflect(-light_direction, normalized_normal);
	int shininess = 32;
	float spec = pow(max(dot(view_direction, reflect_direction), 0.0f), shininess);
	vec3 specular = specular_strength * spec * light_color;
	vec4 result = vec4(ambient + diffuse + specular, 1.0f) * output_color_;*/

	out_color_ = texture(texture_sampler0_, output_text_coord_);
	//out_color_ = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//out_color_ = result;
}