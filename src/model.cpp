#include "model.h"

#include <iostream>

Model::Model(std::string_view path)
	: path_(path)
{
	open_gltf_file();
	print_info_gltf();
	load_meshes();
}

Model::~Model()
{
	tg3_model_free(&model_);
	tg3_error_stack_free(&errors_);
}

void Model::open_gltf_file()
{
	tg3_parse_options opts;
	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors_);

	tg3_error_code err = tg3_parse_file(&model_, &errors_, path_.c_str(), 30, &opts);
	if(err != TG3_OK)
	{
		for(uint32_t i = 0; i < errors_.count; i++)
		{
			std::string str = errors_.entries[i].message ? errors_.entries[i].message : "(null)";
			std::cout << int(errors_.entries[i].severity) << ", " << str << std::endl;
		}
	}
}

void Model::print_info_gltf() const
{
	std::cout << "\n**** Info on the GLTF file/model ****\n";
	std::cout << "- File name: " << path_ << std::endl;
	std::cout << "- The model has " << model_.nodes_count << " node(s), " << model_.meshes_count << " meshe(s), " << model_.buffer_views_count << " buffer view(s) and " << model_.accessors_count << " accessor(s).\n";

	for(uint32_t i = 0; i < model_.nodes_count; ++i)
	{
		for(uint32_t j = 0; j < model_.meshes_count; ++j)
		{
			tg3_mesh mesh = model_.meshes[j];
			std::cout << "- Mesh " << model_.nodes->mesh << ": " << std::endl;
			for(uint32_t k = 0; k < mesh.primitives_count; ++k)
			{
				tg3_primitive primitive = mesh.primitives[k];
				for(uint32_t kk = 0; kk < primitive.attributes_count; kk++)
				{
					if(std::string(primitive.attributes[kk].key.data) != "POSITION") //TODO : ne pas oublier d'enlever ce code
					{
						std::cout << "Attribute: " << std::string(primitive.attributes[kk].key.data) << " -> NOT HANDLED FOR NOW!\n";
					}
					else
					{
						std::cout << "   .Primitive: " << primitive.attributes[kk].key.data << " = " << primitive.attributes[kk].value << std::endl;
					}
				}
				std::cout << "   .Indices: " << primitive.indices << std::endl;
			}
		}
	}

	for(uint32_t i = 0; i < model_.accessors_count; ++i)
	{
		tg3_accessor accessor = model_.accessors[i];
		std::cout << "- Accessor " << i << ": " << std::endl;
		std::cout << "   .Buffer view (index = " << accessor.buffer_view << ", offset = " << accessor.byte_offset << ")\n";
		std::cout << "   .Type = " << accessor.type << ", count = " << accessor.count << ", component type = " << get_component_type_str(accessor.component_type) << std::endl;
	}

	for(uint32_t i = 0; i < model_.buffer_views_count; ++i)
	{
		tg3_buffer_view buffer_view = model_.buffer_views[i];
		std::cout << "- Buffer view " << i << ": " << std::endl;
		std::cout << "   .Buffer indice = " << buffer_view.buffer << std::endl;
		std::cout << "   .Length = " << buffer_view.byte_length << ", offset = " << buffer_view.byte_offset << ", target = " << get_target_str(buffer_view.target) << std::endl;
	}

	for(uint32_t i = 0; i < model_.buffers_count; ++i)
	{
		tg3_buffer buffer = model_.buffers[i];
		std::cout << "- Buffer " << i << ": " << std::endl;
		std::cout << "   .Length = " << buffer.data.count << std::endl;
	}

	std::cout << "*************************************\n\n";
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
			component_type_str = "GL_FLOAT";
			break;

		default:
			component_type_str = "****ERROR****";
			break;
	}
	return component_type_str;
}

//TODO : free function
std::size_t Model::get_component_type_size(int32_t component_type) const
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
			component_type_size = 0;
			break;
	}
	return component_type_size;
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

	std::vector<GLushort> ebo_value;
	ebo_value.reserve(accessor.count);

	//affiche tous les indices => devront être stockés dans un EBO 
	for(uint64_t i = buffer_view.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += sizeof(GLushort))
	{
		//TODO : utiliser une boucle comme pour "position"
		GLushort indice_value = (buffer.data.data[i + 1] << 8) | buffer.data.data[i]; //=> little-endian
		//std::cout << +indice_value << std::endl; //"+" pour que l'élément ne soit pas considéré comme un char mais comme un int
		ebo_value.push_back(indice_value);
	}

	return ebo_value;
}

std::vector<glm::vec2> Model::get_vec2_attribute(const tg3_str_int_pair& attribute) const
{
	tg3_accessor accessor = model_.accessors[attribute.value];
	tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
	tg3_buffer buffer = model_.buffers[buffer_view.buffer];
	std::string component_type_str = get_component_type_str(accessor.component_type);
	std::size_t component_type_size = get_component_type_size(accessor.component_type);
	std::string type_str = get_type_str(accessor.type);

	if(get_component_type_str(accessor.component_type) != "GL_FLOAT")
	{
		std::cout << "****ERROR****\n";
	}

	std::vector<glm::vec2> vec2_vector;
	if(type_str == "VEC2")
	{
		for(uint64_t i = buffer_view.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += sizeof(glm::vec2))
		{
			glm::vec2 vec2 = glm::vec2(0.0f, 0.0f);
			for(uint64_t j = 0; j < component_type_size * 2; j += component_type_size) //2 car vec2
			{
				uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 octets
				for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
				{
					attribute_ieee754 |= uint32_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
				}
				GLfloat attribute_float = ieee754_to_float(attribute_ieee754);

				//TODO : créer une fonction qui prend 0 ou 1 en paramètre et positionne x ou y d'un vec2 ??
				if(j == component_type_size * 0) //x
				{
					vec2.x = attribute_float;
				}
				else if(j == component_type_size * 1) //y
				{
					vec2.y = attribute_float;
				}
			}
			vec2_vector.push_back(vec2);
		}
	}
	else
	{
		std::cout << "****ERROR****\n";
	}
	return vec2_vector;
}

std::vector<glm::vec3> Model::get_vec3_attribute(const tg3_str_int_pair& attribute) const
{
	tg3_accessor accessor = model_.accessors[attribute.value];
	tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
	tg3_buffer buffer = model_.buffers[buffer_view.buffer];
	std::string component_type_str = get_component_type_str(accessor.component_type);
	std::size_t component_type_size = get_component_type_size(accessor.component_type);
	std::string type_str = get_type_str(accessor.type);

	if(get_component_type_str(accessor.component_type) != "GL_FLOAT")
	{
		std::cout << "****ERROR****\n";
	}

	std::vector<glm::vec3> vec3_vector;
	if(type_str == "VEC3")
	{
		for(uint64_t i = buffer_view.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += sizeof(glm::vec3))
		{
			glm::vec3 vec3 = glm::vec3(0.0f, 0.0f, 0.0f);
			for(uint64_t j = 0; j < component_type_size * 3; j += component_type_size) //3 car vec3
			{
				uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 octets
				for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
				{
					attribute_ieee754 |= uint32_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
				}
				GLfloat attribute_float = ieee754_to_float(attribute_ieee754);

				//TODO : créer une fonction qui prend 0, 1 ou 2 en paramètre et positionne x, y ou z d'un vec3 ??
				if(j == component_type_size * 0) //x
				{
					vec3.x = attribute_float;
				}
				else if(j == component_type_size * 1) //y
				{
					vec3.y = attribute_float;
				}
				else if(j == component_type_size * 2) //z
				{
					vec3.z = attribute_float;
				}
			}
			vec3_vector.push_back(vec3);
		}
	}
	else
	{
		std::cout << "****ERROR****\n";
	}
	return vec3_vector;
}

//TODO : free function
//gère POSITION, NORMAL, TEXCOORD_n et COLOR_n
//std::vector<Mesh::Vertex> Model::get_vertex_attributes(const tg3_str_int_pair& attribute) const
//{
//	tg3_accessor accessor = model_.accessors[attribute.value];
//	tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
//	tg3_buffer buffer = model_.buffers[buffer_view.buffer];
//	std::string attribute_name_str = std::string(attribute.key.data);
//
//	std::vector<Mesh::Vertex> vertex_attributes;
//	std::vector<glm::vec3> position_vector;
//	std::vector<glm::vec3> normal_vector;
//	std::vector<glm::vec2> texcoord_vector;
//	std::vector<glm::vec3> color_vector;
//
//	//TODO : attention car cette méthode ne gère qu'un attribut à la fois !!
//
//	if(attribute_name_str == "POSITION") //vec3 de float
//	{
//		position_vector = get_vec3_attribute(attribute);
//
//		for(const glm::vec3& vec3 : position_vector)
//		{
//			//TODO : réserver de la place dans le vector ?? => accessor.count d'un des attributs car tous les attributs sont logiquement au même nombre
//		}
//	}
//	else if(attribute_name_str == "NORMAL") //vec3 de float
//	{
//		normal_vector = get_vec3_attribute(attribute);
//	}
//	else if(attribute_name_str.find("TEXCOORD_") != std::string::npos) //vec2
//	{
//		texcoord_vector = get_vec2_attribute(attribute);
//	}
//	else if(attribute_name_str.find("COLOR_") != std::string::npos) //vec3 ou vec4
//	{
//		if(get_type_str(accessor.type) == "VEC3")
//		{
//			color_vector = get_vec3_attribute(attribute);
//		}
//		else if(get_type_str(accessor.type) == "VEC4")
//		{
//			std::cout << "****ERROR****\n";
//		}
//	}
//	else
//	{
//		std::cout << "****ERROR****\n";
//	}
//	return vertex_attributes;
//}

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
				std::vector<GLushort> ebo_values = get_ebo_values(primitive);
				std::vector<Mesh::Vertex> vertex_attributes;

				std::vector<glm::vec3> position_vector;
				std::vector<glm::vec3> normal_vector;
				std::vector<glm::vec2> texcoord_vector;
				std::vector<glm::vec3> color_vector;

				for(uint32_t kk = 0; kk < primitive.attributes_count; kk++)
				{
					tg3_str_int_pair attribute = primitive.attributes[kk];
					std::string attribute_name_str = std::string(attribute.key.data);
					tg3_accessor accessor = model_.accessors[attribute.value];

					if(attribute_name_str == "POSITION") //vec3 de float
					{
						position_vector = get_vec3_attribute(attribute);
					}
					else if(attribute_name_str == "NORMAL") //vec3 de float
					{
						normal_vector = get_vec3_attribute(attribute);
					}
					else if(attribute_name_str.find("TEXCOORD_") != std::string::npos) //vec2
					{
						texcoord_vector = get_vec2_attribute(attribute);
					}
					else if(attribute_name_str.find("COLOR_") != std::string::npos) //vec3 ou vec4
					{
						if(get_type_str(accessor.type) == "VEC3")
						{
							color_vector = get_vec3_attribute(attribute);
						}
						else if(get_type_str(accessor.type) == "VEC4")
						{
							std::cout << "****ERROR****\n";
						}
					}
					else
					{
						std::cout << "****ERROR****\n";
					}
				}

				/*for(GLushort ebo : ebo_values)
				{
					std::cout << ebo << ", ";
				}
				std::cout << std::endl;*/

				vertex_attributes.reserve(position_vector.size()); //position_vector est normalement toujours rempli

				if(normal_vector.size() == 0)
				{
					normal_vector.resize(position_vector.size(), glm::vec3(0.0f, 0.0f, 0.0f));
				}

				if(color_vector.size() == 0)
				{
					color_vector.resize(position_vector.size(), glm::vec3(0.0f, 0.0f, 0.0f));
				}

				if(texcoord_vector.size() == 0)
				{
					texcoord_vector.resize(position_vector.size(), glm::vec2(0.0f, 0.0f));
				}

				for(size_t i = 0; i < position_vector.size(); ++i)
				{
					Mesh::Vertex vertex(position_vector[i], color_vector[i], texcoord_vector[i]); 
					vertex_attributes.push_back(vertex);
				}

				/*for(const Mesh::Vertex& v : vertex_attributes)
				{
					std::cout << "(.x: " << v.color_.x << ", .y: " << v.color_.y << ", .z: " << v.color_.z << ")\n";
					std::cout << "(.x: " << v.position_.x << ", .y: " << v.position_.y << ", .z: " << v.position_.z << ")\n";
					std::cout << "(.x: " << v.texture_coordinates_.x << ", .y: " << v.texture_coordinates_.y << ")\n";
				}*/

				meshes_.push_back(Mesh(ebo_values, vertex_attributes, primitive.mode)); 
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