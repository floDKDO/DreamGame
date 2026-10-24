#version 450 core

layout (location = 0) in vec3 position_attribute_;

out vec3 texcoord_;

uniform mat4 projection_matrix_;
uniform mat4 view_matrix_;

void main()
{
	gl_Position = projection_matrix_ * view_matrix_ * vec4(position_attribute_, 1.0);
	texcoord_ = position_attribute_;
}