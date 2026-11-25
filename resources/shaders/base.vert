#version 450 core

layout (location = 0) in vec3 position_attribute_;
layout (location = 1) in vec3 normal_attribute_;
layout (location = 2) in vec2 texcoord_attribute_;
layout (location = 3) in vec4 color_attribute_;

out vec4 color_;
out vec2 texcoord_;

void main()
{
    gl_Position = vec4(position_attribute_, 1.0); 
    color_ = color_attribute_;
	texcoord_ = texcoord_attribute_;
}  