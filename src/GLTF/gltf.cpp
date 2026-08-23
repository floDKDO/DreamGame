#include "gltf.h"

#include <GL/glew.h>
#include <tinygltf/tiny_gltf_v3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace gltf
{

glm::mat4 get_mat4_from_1d_matrix(const double m[16])
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

glm::mat4 get_transformation_matrix(glm::mat4 parent_matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale)
{
	glm::mat4 matrix_mat4(1.0f);
	matrix_mat4 *= parent_matrix; //cette multiplication doit être effectuée en première
	matrix_mat4 = glm::translate(matrix_mat4, translation);
	matrix_mat4 *= glm::mat4_cast(rotation);
	matrix_mat4 = glm::scale(matrix_mat4, scale);
	return matrix_mat4;
}

glm::mat4 get_transformation_matrix_from_double(glm::mat4 parent_matrix, const double translation[3], const double rotation[4], const double scale[3])
{
	glm::vec3 translation_vec3;
	translation_vec3.x = float(translation[0]);
	translation_vec3.y = float(translation[1]);
	translation_vec3.z = float(translation[2]);

	glm::quat rotation_quat;
	rotation_quat.w = float(rotation[3]);
	rotation_quat.x = float(rotation[0]);
	rotation_quat.y = float(rotation[1]);
	rotation_quat.z = float(rotation[2]);

	glm::vec3 scale_vec3;
	scale_vec3.x = float(scale[0]);
	scale_vec3.y = float(scale[1]);
	scale_vec3.z = float(scale[2]);

	return get_transformation_matrix(parent_matrix, translation_vec3, rotation_quat, scale_vec3);
}

//TODO : voir #include <glm/gtx/io.hpp> qui surcharge l'opérateur "<<"
void print_mat4(glm::mat4 m)
{
	for(glm::length_t i = 0; i < 4; ++i)
	{
		for(glm::length_t j = 0; j < 4; ++j)
		{
			std::cout << m[j][i] << ", \t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void print_1d_matrix(const double m[16])
{
	for(glm::length_t i = 0; i < 4; ++i)
	{
		for(glm::length_t j = 0; j < 4 * 4; j += 4)
		{
			std::cout << m[i + j] << ", \t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
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

std::vector<glm::vec4> vec3_to_vec4_colors(std::vector<glm::vec3> vector)
{
	std::vector<glm::vec4> vector_vec4;
	vector_vec4.reserve(vector.size());
	for(const glm::vec3& v : vector)
	{
		vector_vec4.push_back(glm::vec4(v, 1.0f)); //1.0f = alpha
	}
	return vector_vec4;
}

}