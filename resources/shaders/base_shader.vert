#version 450 core

layout (location = 0) in vec3 position_attribute_;
layout (location = 1) in vec3 normal_attribute_;
layout (location = 2) in vec2 texcoord_attribute_;
layout (location = 3) in vec4 color_attribute_;

out vec4 color_;
out vec2 texcoord_;
out vec3 normal_;
out vec3 world_position_;

uniform mat4 model_matrix_;
uniform mat4 view_matrix_;
uniform mat4 projection_matrix_;

void main()
{
	gl_Position = projection_matrix_ * view_matrix_ * model_matrix_ * vec4(position_attribute_, 1.0f);
	color_ = color_attribute_;
	texcoord_ = texcoord_attribute_;
	normal_ = mat3(transpose(inverse(model_matrix_))) * normal_attribute_;
	world_position_ = vec3(model_matrix_ * vec4(position_attribute_, 1.0f));
}