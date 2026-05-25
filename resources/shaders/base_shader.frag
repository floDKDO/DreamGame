#version 450 core

in vec3 output_color_;
in vec2 output_text_coord_;
out vec4 out_color_;

uniform sampler2D texture_sampler0_;
uniform sampler2D texture_sampler1_;

void main()
{
	out_color_ = mix(texture(texture_sampler0_, output_text_coord_), texture(texture_sampler1_, output_text_coord_), 0.5);// * vec4(output_color_.x, output_color_.y, output_color_.z, 1.0f);
}