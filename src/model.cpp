#include "model.h"

#include <iostream>

Model::Model(std::string_view path)
{
	open_gltf_file(path);
	load_meshes();
}

Model::~Model()
{
	tg3_model_free(&model_);
	tg3_error_stack_free(&errors_);
}

void Model::open_gltf_file(std::string_view path)
{
	std::string path_str = std::string(path);

	tg3_parse_options opts;
	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors_);

	tg3_error_code err = tg3_parse_file(&model_, &errors_, path_str.c_str(), 30, &opts);
	if(err != TG3_OK)
	{
		for(uint32_t i = 0; i < errors_.count; i++)
		{
			std::string str = errors_.entries[i].message ? errors_.entries[i].message : "(null)";
			std::cout << int(errors_.entries[i].severity) << ", " << str << std::endl;
		}
	}
}

//TODO : free function
std::string Model::get_target_str(int32_t target) const
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
			target_str = "****ERROR****";
			break;
	}
	return target_str;
}

//TODO : free function
std::string Model::get_type_str(int32_t type) const
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
			type_str = "****ERROR****";
			break;
	}
	return type_str;
}

//TODO : free function
std::string Model::get_component_type_str(int32_t component_type) const
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
			component_type_str = "*GL_FLOAT";
			break;

		default:
			component_type_str = "****ERROR****";
			break;
	}
	return component_type_str;
}

//TODO : free function
GLfloat Model::ieee754_to_float(uint64_t ieee754_number) const
{
	GLfloat float_number;
	std::memcpy(&float_number, &ieee754_number, sizeof(float_number)); //obligé de faire cela pour convertir un nombre IEEE-754 en float (voir https://stackoverflow.com/questions/56710780/how-is-1-encoded-in-c-c-as-a-float-assuming-ieee-754-single-precision-represe)
	return float_number;
}

//TODO : free function
std::vector<GLushort> Model::get_ebo_values(const tg3_primitive& primitive) const
{
	tg3_accessor accessor = model_.accessors[primitive.indices];
	tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
	tg3_buffer buffer = model_.buffers[buffer_view.buffer];

	std::cout << "Target: " << get_target_str(buffer_view.target) << std::endl;
	std::cout << "Type: " << get_type_str(accessor.type) << std::endl;
	std::cout << "Component type: " << get_component_type_str(accessor.component_type) << std::endl;

	std::vector<GLushort> ebo_value;
	ebo_value.reserve(accessor.count);

	//affiche tous les indices => devront être stockés dans un EBO 
	for(uint64_t i = buffer_view.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += sizeof(GLushort))
	{
		//TODO : utiliser une boucle comme pour "position"
		GLushort indice_value = (buffer.data.data[i + 1] << 8) | buffer.data.data[i]; //=> little-endian
		std::cout << +indice_value << std::endl; //"+" pour que l'élément ne soit pas considéré comme un char mais comme un int
		ebo_value.push_back(indice_value);
	}

	return ebo_value;
}

//TODO : free function
std::vector<glm::vec3> Model::get_position_attribute_values(const tg3_str_int_pair& attribute) const
{
	tg3_accessor accessor = model_.accessors[attribute.value];
	tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
	tg3_buffer buffer = model_.buffers[buffer_view.buffer];

	std::cout << "Target: " << get_target_str(buffer_view.target) << std::endl;
	std::cout << "Type: " << get_type_str(accessor.type) << std::endl;
	if(std::string(attribute.key.data) != "POSITION")
	{
		std::cout << "Attribute: NOT HANDLED FOR NOW!\n";
	}
	std::cout << "Attribute: " << attribute.key.data << std::endl;

	std::vector<glm::vec3> position_vector;
	glm::vec3 position_vec3;
	int vector_counter = 0; //pour créer le vec3

	//afficher l'attribut position (floats qui sont des composants d'un vec3)
	for(uint64_t i = buffer_view.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += sizeof(GLfloat))
	{
		//uint64_t position = 0;
		//size_t quotient_size = sizeof(GLfloat) / sizeof(uint8_t);
		//for(uint8_t b = 1; b <= quotient_size; ++b) //TODO : voir pour unifier ce code avec la variable "indice_value"
		//{
		//	position |= uint64_t(buffer.data.data[i + quotient_size - b]) << (sizeof(GLfloat) * 8 - (sizeof(uint8_t) * b)); //=> little-endian
		//}

		uint64_t position = (buffer.data.data[i + 3] << 24) 
			| (buffer.data.data[i + 2] << 16)
			| (buffer.data.data[i + 1] << 8)
			| buffer.data.data[i];

		GLfloat position_float = ieee754_to_float(position);
		std::cout << "Position: " << std::hex << position << std::dec << ", position float: " << position_float << std::endl;

		if(vector_counter == 0) //x
		{
			position_vec3.x = position_float;
		}
		else if(vector_counter == 1) //y
		{
			position_vec3.y = position_float;
		}
		else if(vector_counter == 2) //z
		{
			position_vec3.z = position_float;
		}
		vector_counter += 1;

		if(vector_counter == 3)
		{
			position_vector.push_back(position_vec3);
			position_vec3 = glm::vec3(0.0f, 0.0f, 0.0f);
			vector_counter = 0;
		}
	}
	return position_vector;
}

void Model::load_meshes()
{
	if(model_.nodes_count > 1)
	{
		std::cout << "WARNING: multiple nodes not handled for now!\n";
	}

	//for(uint32_t i = 0; i < model_.nodes_count; ++i)
	{
		for(uint32_t j = 0; j < model_.meshes_count; ++j)
		{
			tg3_mesh mesh = model_.meshes[j];
			for(uint32_t k = 0; k < mesh.primitives_count; ++k)
			{
				tg3_primitive primitive = mesh.primitives[k];
				for(uint32_t kk = 0; kk < primitive.attributes_count; kk++)
				{
					std::vector<GLushort> ebo_values = get_ebo_values(primitive);
					std::vector<glm::vec3> position_attribute_values = get_position_attribute_values(primitive.attributes[kk]);
					meshes_.push_back(Mesh(ebo_values, position_attribute_values, primitive.mode));

					/*for(GLushort ebo : ebo_values)
					{
						std::cout << ebo << ", ";
					}

					for(glm::vec3 v : position_attribute_values)
					{
						std::cout << "(.x: " << v.x << ", .y: " << v.y << ", .z: " << v.z << "), ";
					}*/
				}
			}
		}
	}
}

void Model::draw(ShaderProgram& shader_program)
{
	for(Mesh& mesh : meshes_)
	{
		mesh.draw(shader_program);
	}
}