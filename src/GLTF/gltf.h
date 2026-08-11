#pragma once

#include <GL/glew.h>
#include <glm/mat4x4.hpp>
#include <string>

namespace gltf
{

glm::mat4 get_mat4_from_1d_matrix(double m[16]);
glm::mat4 get_transformation_matrix(double rotation[4], double scale[3], double translation[3]);
void print_mat4(glm::mat4 m);
void print_1d_matrix(double m[16]);
bool is_mat4_identity(glm::mat4 m);
bool is_1d_matrix_identity(double m[16]);
std::string get_target_str(int32_t target);
std::string get_type_str(int32_t type);
std::string get_component_type_str(int32_t component_type);
std::size_t get_component_type_size(int32_t component_type);
std::string get_filter_str(int32_t filter);
std::string get_wrap_str(int32_t wrap);

}