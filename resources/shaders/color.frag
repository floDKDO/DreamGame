#version 450 core

in vec4 color_;
out vec4 out_color_;

uniform float alpha_;

void main()
{ 
    out_color_ = vec4(color_.xyz, alpha_);
}