#version 450 core

layout (location = 0) in vec3 position_attribute_;
layout (location = 2) in vec2 texcoord_attribute_;

out vec2 texcoord_;

void main()
{
    gl_Position = vec4(position_attribute_, 1.0); 
    texcoord_ = texcoord_attribute_;
}  