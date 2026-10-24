#version 450 core

in vec3 texcoord_;
out vec4 out_color_;

uniform samplerCube cubemap_sampler0_;

void main()
{
	out_color_ = texture(cubemap_sampler0_, texcoord_);
}