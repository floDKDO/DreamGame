#include "node.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>

Node::Node(std::string_view path)
	: path_(path), mesh_index_(-1), //model_matrix_(glm::mat4(1.0f)), 
	position_(0.0f, 0.0f, 0.0f), 
	rotation_(0.0f, 0.0f, 0.0f, 1.0f), rotation_info_({0.0f, glm::vec3(0.0f, 1.0f, 0.0f)}),
	scale_(1.0f, 1.0f, 1.0f)
{
	open_gltf_file();
	print_info_gltf();
	load_meshes();
}

void Node::open_gltf_file()
{
	tg3_parse_options opts;
	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors_);

	tg3_error_code err = tg3_parse_file(&model_, &errors_, path_.c_str(), uint32_t(path_.length()), &opts);
	if(err != TG3_OK)
	{
		for(uint32_t i = 0; i < errors_.count; i++)
		{
			std::string str = errors_.entries[i].message ? errors_.entries[i].message : "(null)";
			std::cout << int(errors_.entries[i].severity) << ", " << str << std::endl;
		}
	}
}

void Node::print_info_gltf() const
{
	std::cout << "\n**** Info on the GLTF file/model ****\n";
	std::cout << "- File name: " << path_ << std::endl;
	std::cout << "- The model has " << model_.nodes_count << " node(s), " << model_.meshes_count << " meshe(s), " << model_.buffer_views_count << " buffer view(s), " << model_.accessors_count << " accessor(s), " << model_.textures_count << " texture(s), " << model_.samplers_count << " sampler(s) and " << model_.images_count << " image(s).\n";

	for(uint32_t i = 0; i < model_.nodes_count; ++i)
	{
		tg3_node node = model_.nodes[i];
		std::cout << "- Node " << i << ": " << std::endl;
		std::cout << "   .Mesh index: " << node.mesh << ", rotation: (" << node.rotation[0] << ", " << node.rotation[1] << ", " << node.rotation[2] << ", " << node.rotation[3] <<
			"), scale: (" << node.scale[0] << ", " << node.scale[1] << ", " << node.scale[2] <<
			"), translation: (" << node.translation[0] << ", " << node.translation[1] << ", " << node.translation[2] << ")\n";

		tg3_mesh mesh = model_.meshes[node.mesh];
		std::cout << "- Mesh " << model_.nodes->mesh << ": " << std::endl;
		for(uint32_t k = 0; k < mesh.primitives_count; ++k)
		{
			tg3_primitive primitive = mesh.primitives[k];
			for(uint32_t kk = 0; kk < primitive.attributes_count; kk++)
			{
				std::cout << "   .Primitive: " << primitive.attributes[kk].key.data << " = " << primitive.attributes[kk].value << std::endl;
			}
			std::cout << "   .Indices: " << primitive.indices << std::endl;
		}
	}

	for(uint32_t i = 0; i < model_.accessors_count; ++i)
	{
		tg3_accessor accessor = model_.accessors[i];
		std::cout << "- Accessor " << i << ": " << std::endl;
		std::cout << "   .Buffer view (index = " << accessor.buffer_view << ", offset = " << accessor.byte_offset << ")\n";
		std::cout << "   .Type = " << gltf::get_type_str(accessor.type) << ", count = " << accessor.count << ", component type = " << gltf::get_component_type_str(accessor.component_type) << std::endl;
	}

	for(uint32_t i = 0; i < model_.buffer_views_count; ++i)
	{
		tg3_buffer_view buffer_view = model_.buffer_views[i];
		std::cout << "- Buffer view " << i << ": " << std::endl;
		std::cout << "   .Buffer indice = " << buffer_view.buffer << std::endl;
		std::cout << "   .Length = " << buffer_view.byte_length << ", offset = " << buffer_view.byte_offset << ", target = " << gltf::get_target_str(buffer_view.target) << std::endl;
	}

	for(uint32_t i = 0; i < model_.buffers_count; ++i)
	{
		tg3_buffer buffer = model_.buffers[i];
		std::cout << "- Buffer " << i << ": " << std::endl;
		std::cout << "   .Length = " << buffer.data.count << std::endl;
	}

	for(uint32_t i = 0; i < model_.textures_count; ++i)
	{
		tg3_texture texture = model_.textures[i];
		std::cout << "- Texture " << i << ": " << std::endl;
		std::cout << "   .Source = " << texture.source << ", sampler = " << texture.sampler << std::endl;
	}

	for(uint32_t i = 0; i < model_.samplers_count; ++i)
	{
		tg3_sampler sampler = model_.samplers[i];
		std::cout << "- Sampler " << i << ": " << std::endl;
		std::cout << "   .MagFilter = " << gltf::get_filter_str(sampler.mag_filter) << ", minFilter = " << gltf::get_filter_str(sampler.min_filter) << ", wrapS = " << gltf::get_wrap_str(sampler.wrap_s) << ", wrapT = " << gltf::get_wrap_str(sampler.wrap_t) << std::endl;
	}

	for(uint32_t i = 0; i < model_.images_count; ++i)
	{
		tg3_image image = model_.images[i];
		std::cout << "- Image " << i << ": " << std::endl;
		if(image.uri.len > 0)
		{
			std::cout << "   .URI " << "(Data URI: " << std::boolalpha << bool(tg3_is_data_uri(image.uri.data, image.uri.len)) << std::noboolalpha << ") = " << image.uri.data << std::endl;
		}
		else if(image.buffer_view != -1)
		{
			std::cout << "   .Buffer view = " << image.buffer_view << std::endl;
			std::cout << "   .Mime type = " << image.mime_type.data << std::endl;
		}
	}

	std::cout << "*********************************************************************************************\n\n";
}

glm::mat4 Node::compute_model(/*glm::vec3 translation_vector, float angle, glm::vec3 axis*/)
{
	glm::mat4 model_matrix = glm::mat4(1.0f); //reset de la matrice model à chaque frame
	model_matrix = glm::translate(model_matrix, position_);

	//TODO : rotation (vérifier)
	//model_matrix *= glm::mat4_cast(glm::quat(rotation_));
	model_matrix = glm::rotate(model_matrix, rotation_info_.angle_, rotation_info_.axis_);

	model_matrix = glm::scale(model_matrix, scale_);

	return model_matrix;

	/*model_matrix_ = glm::mat4(1.0f); //reset de la matrice model à chaque frame
	model_matrix_ = glm::translate(model_matrix_, translation_vector);
	model_matrix_ = glm::rotate(model_matrix_, glm::radians(angle), axis);*/
}

void Node::load_meshes()
{
	std::vector<Texture> textures;
	for(uint32_t i = 0; i < model_.textures_count; ++i)
	{
		Texture mesh_texture;
		tg3_texture texture = model_.textures[i];
		tg3_image image = model_.images[texture.source];
		tg3_sampler sampler = model_.samplers[texture.sampler];

		mesh_texture.texture_unit_ = i;
		if(image.buffer_view == -1)
		{
			std::string image_str = std::string(image.uri.data);
			if(bool(tg3_is_data_uri(image.uri.data, image.uri.len)))
			{
				mesh_texture.image_path_ = "";
				//std::string image_data_base64 = image_str.substr(image_str.find(',') + 1); //+1 pour ne pas prendre la virgule
				std::cout << "****ERROR****: Embedded GLTF not handled for now!\n";
			}
			else
			{
				mesh_texture.image_path_ = "resources/models/" + image_str;
			}
		}
		else
		{
			mesh_texture.image_path_ = "";
			tg3_buffer_view buffer_view = model_.buffer_views[image.buffer_view];
			tg3_buffer buffer = model_.buffers[buffer_view.buffer];
			mesh_texture.image_data_.reserve(buffer_view.byte_length);

			//std::copy(buffer.data.data, buffer.data.data + buffer_view.byte_length, std::back_inserter(mesh_texture.image_data_)); //TODO : mieux que la boucle for suivante ?
			for(uint64_t j = 0; j < buffer_view.byte_length; ++j)
			{
				mesh_texture.image_data_.push_back(buffer.data.data[buffer_view.byte_offset + j]);
			}
		}
		mesh_texture.min_filter_ = sampler.min_filter;
		mesh_texture.mag_filter_ = sampler.mag_filter;
		mesh_texture.wrap_s_ = sampler.wrap_s;
		mesh_texture.wrap_t_ = sampler.wrap_t;

		textures.push_back(mesh_texture);
	}

	if(model_.nodes_count != 1)
	{
		std::cerr << "Warning: multiple nodes for a node! Only taking the first node.\n";
	}

	tg3_node node = model_.nodes[0];
	if(gltf::is_1d_matrix_identity(node.matrix)) //=> ignorer la matrice si elle est la matrice identité
	{
		position_ = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
		rotation_ = glm::vec4(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
		scale_ = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
	}
	else
	{
		//nodes_.push_back({Node(node.mesh, node.matrix), glm::vec3(node.translation[0], node.translation[1], node.translation[2])});

		//TODO : à vérifier
		//position_ = glm::vec3(node.matrix[3], node.matrix[7], node.matrix[11]);

		//glm::mat4 model_matrix = glm::mat4(1.0f);
		//model_matrix = glm::decompose(model_matrix, scale_, glm::quat(rotation_), position_, glm::vec3(1.0f), glm::vec4(1.0f));
		//rotation_ = glm::conjugate(glm::quat(rotation_));
		//gltf::print_mat4(model_matrix);
		std::cerr << "************************CAS PAS ENCORE GERE************************\n";
	}

	tg3_mesh mesh = model_.meshes[node.mesh];
	for(uint32_t k = 0; k < mesh.primitives_count; ++k)
	{
		tg3_primitive primitive = mesh.primitives[k];
		std::vector<GLushort> ebo_values = get_ebo_values(primitive);
		Vertices vertices = get_vertices(primitive);
		meshes_.push_back(Mesh(ebo_values, vertices, textures, primitive.mode));

		/*for(GLushort ebo : ebo_values)
		{
			std::cout << ebo << ", ";
		}
		std::cout << std::endl;*/

		//vertices.print();
	}
}

//count devrait être égal pour tous les attributs donc on peut prendre le count de l'attribut [0]
uint64_t Node::get_attributes_count(const tg3_primitive& primitive) const
{
	if(primitive.attributes_count > 0)
	{
		return model_.accessors[primitive.attributes[0].value].count;
	}
	else
	{
		return 0;
	}
}

std::vector<GLushort> Node::get_ebo_values(const tg3_primitive& primitive) const
{
	tg3_accessor accessor = model_.accessors[primitive.indices];
	tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
	tg3_buffer buffer = model_.buffers[buffer_view.buffer];

	std::vector<GLushort> ebo_value;
	ebo_value.reserve(accessor.count);

	for(uint64_t i = buffer_view.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += sizeof(GLushort))
	{
		GLushort indice_value = (buffer.data.data[i + 1] << 8) | buffer.data.data[i]; //=> little-endian
		ebo_value.push_back(indice_value);
	}
	return ebo_value;
}

std::vector<glm::vec4> Node::get_vec4_color_attribute(const tg3_str_int_pair& attribute) const
{
	tg3_accessor accessor = model_.accessors[attribute.value];
	std::string component_type_str = gltf::get_component_type_str(accessor.component_type);
	std::vector<glm::vec4> vec4_vector;

	if(component_type_str == "GL_FLOAT")
	{
		return get_float_vec_attribute<4>(attribute);
	}
	else
	{
		tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
		tg3_buffer buffer = model_.buffers[buffer_view.buffer];
		std::size_t component_type_size = gltf::get_component_type_size(accessor.component_type);

		if(component_type_str == "GL_UNSIGNED_BYTE")
		{
			for(uint64_t i = buffer_view.byte_offset + accessor.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += 4 * sizeof(GLubyte)) //car sizeof(glm::vec4) != 4 * sizeof(sizeof(GLubyte))
			{
				glm::vec4 vec4 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
				for(uint64_t j = 0; j < component_type_size * glm::vec4::length(); j += component_type_size)
				{
					uint8_t attribute_u8 = 0; //uint8_t car un unsigned short fait 8 bits
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_u8 |= uint8_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
					}
					GLfloat attribute_float = GLfloat(attribute_u8) / std::numeric_limits<GLubyte>::max(); //normaliser la valeur de l'attribut
					uint64_t component = j / component_type_size;
					vec4[component % 4] = attribute_float;
				}
				vec4_vector.push_back(vec4);
			}
		}
		else if(component_type_str == "GL_UNSIGNED_SHORT")
		{
			for(uint64_t i = buffer_view.byte_offset + accessor.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += 4 * sizeof(GLushort)) //car sizeof(glm::vec4) != 4 * sizeof(sizeof(GLushort))
			{
				glm::vec4 vec4 = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
				for(uint64_t j = 0; j < component_type_size * glm::vec4::length(); j += component_type_size)
				{
					uint16_t attribute_u16 = 0; //uint16_t car un unsigned short fait 16 bits
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_u16 |= uint16_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
					}
					GLfloat attribute_float = GLfloat(attribute_u16) / std::numeric_limits<GLushort>::max(); //normaliser la valeur de l'attribut
					uint64_t component = j / component_type_size;
					vec4[component % 4] = attribute_float;
				}
				vec4_vector.push_back(vec4);
			}
		}
		else
		{
			std::cout << "****ERROR****: Unhandled type!\n";
		}
	}
	return vec4_vector;
}

std::vector<glm::vec4> Node::vec3_to_vec4_colors(std::vector<glm::vec3> vector) const
{
	std::vector<glm::vec4> vector_vec4;
	vector_vec4.reserve(vector.size());
	for(const glm::vec3& v : vector)
	{
		vector_vec4.push_back(glm::vec4(v, 1.0f)); //1.0f = alpha
	}
	return vector_vec4;
}

Vertices Node::get_vertices(const tg3_primitive& primitive)
{
	Vertices vertices(get_attributes_count(primitive));

	for(uint32_t i = 0; i < primitive.attributes_count; i++)
	{
		tg3_str_int_pair attribute = primitive.attributes[i];
		std::string attribute_name_str = std::string(attribute.key.data);
		tg3_accessor accessor = model_.accessors[attribute.value];

		if(attribute_name_str == "POSITION") //vec3 de float
		{
			vertices.add_position_attributes(get_float_vec_attribute<3>(attribute));
		}
		else if(attribute_name_str == "NORMAL") //vec3 de float
		{
			vertices.add_normal_attributes(get_float_vec_attribute<3>(attribute));
		}
		else if(attribute_name_str.find("TEXCOORD_") != std::string::npos) //vec2
		{
			vertices.add_texture_coordinates_attributes(get_float_vec_attribute<2>(attribute));
		}
		else if(attribute_name_str.find("COLOR_") != std::string::npos) //vec3 ou vec4
		{
			if(gltf::get_type_str(accessor.type) == "VEC3")
			{
				vertices.add_color_attributes(vec3_to_vec4_colors(get_float_vec_attribute<3>(attribute)));
			}
			else if(gltf::get_type_str(accessor.type) == "VEC4")
			{
				vertices.add_color_attributes(get_vec4_color_attribute(attribute));
			}
		}
		else
		{
			std::cout << "****ERROR****: Unknown attribute name!\n";
		}
	}
	return vertices;
}

void Node::draw(ShaderProgram& shader_program)
{
	shader_program.set_uniform_matrix_4fv("model_matrix_", glm::value_ptr(compute_model()));
	for(Mesh& mesh : meshes_)
	{
		mesh.draw(shader_program);
	}
}