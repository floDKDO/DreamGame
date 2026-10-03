#version 450 core

layout (location = 0) in vec2 position_attribute_;
layout (location = 2) in vec2 texcoord_attribute_;
layout (location = 3) in vec4 color_attribute_;

out vec4 color_;
out vec2 texcoord_;

uniform mat4 model_matrix_;
uniform mat4 projection_matrix_;

void main()
{
	gl_Position = projection_matrix_ * model_matrix_ * vec4(position_attribute_, 0.0, 1.0);
	texcoord_ = texcoord_attribute_;
	color_ = color_attribute_;
}