#version 450 core

in vec2 texcoord_;
out vec4 out_color_;

uniform sampler2D screen_texture_sampler0_;

void main()
{ 
    out_color_ = texture(screen_texture_sampler0_, texcoord_);
}