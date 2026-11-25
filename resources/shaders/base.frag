#version 450 core

in vec4 color_;
in vec2 texcoord_;
out vec4 out_color_;

uniform sampler2D texture_sampler0_;

void main()
{
	vec4 texture_color = texture(texture_sampler0_, texcoord_);
	if(texture_color.a < 0.1)
	{
		discard;
	}
	out_color_ = texture_color * color_;
}