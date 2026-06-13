#version 450 core

layout (location = 0) in vec3 position_;
layout (location = 1) in vec3 normal_;
layout (location = 2) in vec2 texture_coordinates_;
layout (location = 3) in vec3 color_;

out vec3 output_color_;
out vec2 output_text_coord_;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 transformation_matrix;

void main()
{
	gl_Position = projection * view * model * transformation_matrix * vec4(position_, 1.0f);
	output_color_ = color_;
	output_text_coord_ = texture_coordinates_;
}