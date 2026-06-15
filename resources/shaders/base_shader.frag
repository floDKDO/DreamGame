#version 450 core

in vec3 output_color_;
in vec2 output_text_coord_;
out vec4 out_color_;

uniform sampler2D texture_sampler0_;

void main()
{
	out_color_ = texture(texture_sampler0_, output_text_coord_);
	//out_color_ = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}