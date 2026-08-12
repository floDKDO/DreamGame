#include "gltf.h"

#include <GL/glew.h>
#include <tinygltf/tiny_gltf_v3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace gltf
{

glm::mat4 get_mat4_from_1d_matrix(double m[16])
{
	glm::mat4 matrix_mat4(1.0f);
	for(glm::length_t i = 0; i < 4 * 4; i += 4)
	{
		for(glm::length_t j = 0; j < 4; ++j)
		{
			matrix_mat4[i / 4][j] = float(m[i + j]);
		}
	}
	return matrix_mat4;
}

glm::mat4 get_transformation_matrix(double rotation[4], double scale[3], double translation[3])
{
	glm::mat4 matrix_mat4(1.0f);
	glm::vec4 rotation_vec4(rotation[0], rotation[1], rotation[2], rotation[3]);
	glm::vec3 scale_vec3(scale[0], scale[1], scale[2]);
	glm::vec3 translation_vec3(translation[0], translation[1], translation[2]);

	matrix_mat4 = glm::translate(matrix_mat4, translation_vec3);
	if(rotation_vec4.x != 0.0f || rotation_vec4.y != 0.0f || rotation_vec4.z != 0.0f)
	{
		matrix_mat4 = glm::rotate(matrix_mat4, glm::acos(rotation_vec4.w), glm::vec3(rotation_vec4.x, rotation_vec4.y, rotation_vec4.z)); //glm::acos donne l'angle en radian donc pas besoin d'utiliser glm::radians
	}
	matrix_mat4 = glm::scale(matrix_mat4, scale_vec3);

	return matrix_mat4;
}

void print_mat4(glm::mat4 m)
{
	for(glm::length_t i = 0; i < 4; ++i)
	{
		for(glm::length_t j = 0; j < 4; ++j)
		{
			std::cout << m[i][j] << ", \t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print_1d_matrix(double m[16])
{
	for(glm::length_t i = 0; i < 4 * 4; i += 4)
	{
		for(glm::length_t j = 0; j < 4; ++j)
		{
			std::cout << m[i + j] << ", \t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool is_mat4_identity(glm::mat4 m)
{
	glm::mat4 identity(1.0f);
	for(glm::length_t i = 0; i < 4; ++i)
	{
		for(glm::length_t j = 0; j < 4; ++j)
		{
			if(m[i][j] != identity[i][j])
			{
				return false;
			}
		}
	}
	return true;
}

bool is_1d_matrix_identity(double m[16])
{
	glm::mat4 identity(1.0f);
	for(glm::length_t i = 0; i < 4 * 4; i += 4)
	{
		for(glm::length_t j = 0; j < 4; ++j)
		{
			if(float(m[i + j]) != identity[i / 4][j])
			{
				return false;
			}
		}
	}
	return true;
}

std::string get_target_str(int32_t target)
{
	std::string target_str;
	switch(target)
	{
		case GL_ARRAY_BUFFER:
			target_str = "GL_ARRAY_BUFFER";
			break;

		case GL_ELEMENT_ARRAY_BUFFER:
			target_str = "GL_ELEMENT_ARRAY_BUFFER";
			break;

		default:
			target_str = "****ERROR****: Unknown target!";
			break;
	}
	return target_str;
}

std::string get_type_str(int32_t type)
{
	std::string type_str;
	switch(type)
	{
		case TG3_TYPE_SCALAR:
			type_str = "SCALAR";
			break;

		case TG3_TYPE_VEC2:
			type_str = "VEC2";
			break;

		case TG3_TYPE_VEC3:
			type_str = "VEC3";
			break;

		case TG3_TYPE_VEC4:
			type_str = "VEC4";
			break;

		case TG3_TYPE_MAT2:
			type_str = "MAT2";
			break;

		case TG3_TYPE_MAT3:
			type_str = "MAT3";
			break;

		case TG3_TYPE_MAT4:
			type_str = "MAT4";
			break;

		default:
			type_str = "****ERROR****: Unknown type!";
			break;
	}
	return type_str;
}

std::string get_component_type_str(int32_t component_type)
{
	std::string component_type_str;
	switch(component_type)
	{
		case GL_BYTE:
			component_type_str = "GL_BYTE";
			break;

		case GL_UNSIGNED_BYTE:
			component_type_str = "GL_UNSIGNED_BYTE";
			break;

		case GL_SHORT:
			component_type_str = "GL_SHORT";
			break;

		case GL_UNSIGNED_SHORT:
			component_type_str = "GL_UNSIGNED_SHORT";
			break;

		case GL_UNSIGNED_INT:
			component_type_str = "GL_UNSIGNED_INT";
			break;

		case GL_FLOAT:
			component_type_str = "GL_FLOAT";
			break;

		default:
			component_type_str = "****ERROR****: Unknown component type!\n";
			break;
	}
	return component_type_str;
}

std::size_t get_component_type_size(int32_t component_type)
{
	std::size_t component_type_size;
	switch(component_type)
	{
		case GL_BYTE:
			component_type_size = sizeof(GLbyte);
			break;

		case GL_UNSIGNED_BYTE:
			component_type_size = sizeof(GLubyte);
			break;

		case GL_SHORT:
			component_type_size = sizeof(GLshort);
			break;

		case GL_UNSIGNED_SHORT:
			component_type_size = sizeof(GLushort);
			break;

		case GL_UNSIGNED_INT:
			component_type_size = sizeof(GLuint);
			break;

		case GL_FLOAT:
			component_type_size = sizeof(GLfloat);
			break;

		default:
			std::cout << "****ERROR****: Unknown component type size!\n";
			component_type_size = 0;
			break;
	}
	return component_type_size;
}

std::string get_filter_str(int32_t filter)
{
	std::string filter_str;
	switch(filter)
	{
		case GL_NEAREST:
			filter_str = "GL_NEAREST";
			break;

		case GL_LINEAR:
			filter_str = "GL_LINEAR";
			break;

		case GL_NEAREST_MIPMAP_NEAREST:
			filter_str = "GL_NEAREST_MIPMAP_NEAREST";
			break;

		case GL_LINEAR_MIPMAP_NEAREST:
			filter_str = "GL_LINEAR_MIPMAP_NEAREST";
			break;

		case GL_NEAREST_MIPMAP_LINEAR:
			filter_str = "GL_NEAREST_MIPMAP_LINEAR";
			break;

		case GL_LINEAR_MIPMAP_LINEAR:
			filter_str = "GL_LINEAR_MIPMAP_LINEAR";
			break;

		default:
			filter_str = "****ERROR****: Unknown filter!";
			break;
	}
	return filter_str;
}

std::string get_wrap_str(int32_t wrap)
{
	std::string wrap_str;
	switch(wrap)
	{
		case GL_CLAMP_TO_EDGE:
			wrap_str = "GL_CLAMP_TO_EDGE";
			break;

		case GL_MIRRORED_REPEAT:
			wrap_str = "GL_MIRRORED_REPEAT";
			break;

		case GL_REPEAT:
			wrap_str = "GL_REPEAT";
			break;

		default:
			wrap_str = "****ERROR****: Unknown wrap!";
			break;
	}
	return wrap_str;
}

}