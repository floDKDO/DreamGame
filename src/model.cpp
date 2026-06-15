#include "model.h"

#include <iostream>


Model::Model(std::string_view path) //si le modèle vient de Blender, il doit avoir été exporté en ayant coché "Transform: + Y Up"
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
		std::cout << "   .URI = " << image.uri.data << std::endl;
	}

	std::cout << "*********************************************************************************************\n\n";
}

//count devrait être égal pour tous les attributs donc on peut prendre le count de l'attribut [0]
uint64_t Model::get_attributes_count(const tg3_primitive& primitive) const
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
	std::string component_type_str = gltf::get_component_type_str(accessor.component_type);
	std::size_t component_type_size = gltf::get_component_type_size(accessor.component_type);
	std::string type_str = gltf::get_type_str(accessor.type);

	if(gltf::get_component_type_str(accessor.component_type) != "GL_FLOAT")
	{
		std::cout << "****ERROR****: only GL_FLOAT is handled fopr now!\n";
	}

	uint64_t stride = buffer_view.byte_stride != 0 ? buffer_view.byte_stride : sizeof(glm::vec2);

	std::vector<glm::vec2> vec2_vector;
	if(type_str == "VEC2")
	{
		for(uint64_t i = buffer_view.byte_offset + accessor.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += stride)
		{
			glm::vec2 vec2 = glm::vec2(0.0f, 0.0f);
			for(uint64_t j = 0; j < component_type_size * 2; j += component_type_size) //2 car vec2
			{
				uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 octets
				for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
				{
					attribute_ieee754 |= uint32_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
				}
				GLfloat attribute_float = gltf::ieee754_to_float(attribute_ieee754);

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
		std::cout << "****ERROR****: Only VEC2 is handled for now!\n";
	}
	return vec2_vector;
}

std::vector<glm::vec3> Model::get_vec3_attribute(const tg3_str_int_pair& attribute) const
{
	tg3_accessor accessor = model_.accessors[attribute.value];
	tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
	tg3_buffer buffer = model_.buffers[buffer_view.buffer];
	std::string component_type_str = gltf::get_component_type_str(accessor.component_type);
	std::size_t component_type_size = gltf::get_component_type_size(accessor.component_type);
	std::string type_str = gltf::get_type_str(accessor.type);

	if(gltf::get_component_type_str(accessor.component_type) != "GL_FLOAT")
	{
		std::cout << "****ERROR****: Only GL_FLOAT is handled for now!\n";
	}

	uint64_t stride = buffer_view.byte_stride != 0 ? buffer_view.byte_stride : sizeof(glm::vec3);

	std::vector<glm::vec3> vec3_vector;
	if(type_str == "VEC3")
	{
		for(uint64_t i = buffer_view.byte_offset + accessor.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += stride)
		{
			glm::vec3 vec3 = glm::vec3(0.0f, 0.0f, 0.0f);
			for(uint64_t j = 0; j < component_type_size * 3; j += component_type_size) //3 car vec3
			{
				uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 octets
				for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
				{
					attribute_ieee754 |= uint32_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
				}
				GLfloat attribute_float = gltf::ieee754_to_float(attribute_ieee754);

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
		std::cout << "****ERROR****: Only VEC3 is handled for now!\n";
	}
	return vec3_vector;
}

Vertices Model::get_vertices(const tg3_primitive& primitive)
{
	Vertices vertices(get_attributes_count(primitive));

	for(uint32_t i = 0; i < primitive.attributes_count; i++)
	{
		tg3_str_int_pair attribute = primitive.attributes[i];
		std::string attribute_name_str = std::string(attribute.key.data);
		tg3_accessor accessor = model_.accessors[attribute.value];

		if(attribute_name_str == "POSITION") //vec3 de float
		{
			vertices.add_position_attributes(get_vec3_attribute(attribute));
		}
		else if(attribute_name_str == "NORMAL") //vec3 de float
		{
			vertices.add_normal_attributes(get_vec3_attribute(attribute));
		}
		else if(attribute_name_str.find("TEXCOORD_") != std::string::npos) //vec2
		{
			vertices.add_texture_coordinates_attributes(get_vec2_attribute(attribute));
		}
		else if(attribute_name_str.find("COLOR_") != std::string::npos) //vec3 ou vec4
		{
			if(gltf::get_type_str(accessor.type) == "VEC3")
			{
				vertices.add_color_attributes(get_vec3_attribute(attribute));
			}
			else if(gltf::get_type_str(accessor.type) == "VEC4")
			{
				std::cout << "****ERROR****: Only VEC3 is handled for now!\n";
			}
		}
		else
		{
			std::cout << "****ERROR****: Unknown attribute name!\n";
		}
	}
	return vertices;
}

void Model::load_meshes()
{
	std::vector<Texture::TextureInfo> textures;
	for(uint32_t i = 0; i < model_.textures_count; ++i)
	{
		Texture::TextureInfo mesh_texture;
		tg3_texture texture = model_.textures[i];
		tg3_image image = model_.images[texture.source];
		tg3_sampler sampler = model_.samplers[texture.sampler];

		mesh_texture.texture_index_ = i;
		mesh_texture.image_path_ = "resources/models/" + std::string(image.uri.data); 
		mesh_texture.min_filter_ = sampler.min_filter;
		mesh_texture.mag_filter_ = sampler.mag_filter;
		mesh_texture.wrap_s_ = sampler.wrap_s;
		mesh_texture.wrap_t_ = sampler.wrap_t;

		textures.push_back(mesh_texture);
	}

	for(uint32_t i = 0; i < model_.nodes_count; ++i)
	{
		tg3_node node = model_.nodes[i];
		if(gltf::is_1d_matrix_identity(node.matrix))
		{
			nodes_.push_back(Node(node.mesh, node.rotation, node.scale, node.translation));
		}
		else
		{
			nodes_.push_back(Node(node.mesh, node.matrix));
		}

		tg3_mesh mesh = model_.meshes[node.mesh];
		for(uint32_t k = 0; k < mesh.primitives_count; ++k)
		{
			tg3_primitive primitive = mesh.primitives[k];
			std::vector<GLushort> ebo_values = get_ebo_values(primitive);
			Vertices vertices = get_vertices(primitive);
			meshes_.push_back(Mesh(ebo_values, vertices, textures, nodes_.back().transformation_matrix_, primitive.mode)); 

			/*for(GLushort ebo : ebo_values)
			{
				std::cout << ebo << ", ";
			}
			std::cout << std::endl;*/

			/*for(const Mesh::Vertex& v : vertices.vertices_)
			{
				std::cout << "Position : (.x: " << v.position_.x << ", .y: " << v.position_.y << ", .z: " << v.position_.z << ")\n";
				std::cout << "Normal : (.x: " << v.normal_.x << ", .y: " << v.normal_.y << ", .z: " << v.normal_.z << ")\n";
				std::cout << "Texture coordinates : (.x: " << v.texture_coordinates_.x << ", .y: " << v.texture_coordinates_.y << ")\n";
				std::cout << "Color : (.x: " << v.color_.x << ", .y: " << v.color_.y << ", .z: " << v.color_.z << ")\n";
			}*/
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