#include "gltf_file.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

glTFFile::glTFFile(std::string_view path)
	: path_(path)
{
	open();
	print_info();
}

void glTFFile::open()
{
	tg3_parse_options opts;
	tg3_parse_options_init(&opts);
	tg3_error_stack_init(&errors_);

	tg3_error_code err;
	if((err = tg3_parse_file(&model_, &errors_, path_.c_str(), uint32_t(path_.length()), &opts)) != TG3_OK)
	{
		for(uint32_t i = 0; i < errors_.count; i++)
		{
			std::string str = errors_.entries[i].message ? errors_.entries[i].message : "(null)";
			std::cout << int(errors_.entries[i].severity) << ", " << str << std::endl;
		}
	}
}

void glTFFile::print_info() const
{
	std::cout << "\n**** Info on the glTF file ****\n";
	std::cout << "- General information: " << std::endl;
	std::cout << "    - File name: " << path_ << std::endl;
	std::cout << "    - The file contains " << model_.scenes->nodes_count << " root node(s).\n";
	std::cout << "    - There are " << model_.nodes_count << " node(s), " << model_.meshes_count << " mesh(es), " << model_.accessors_count << " accessor(s), " << model_.buffer_views_count << " buffer view(s), " << model_.buffers_count << " buffer(s), " << model_.textures_count << " texture(s), " << model_.samplers_count << " sampler(s) and " << model_.images_count << " image(s).\n";

	std::cout << "- File content: " << std::endl;
	std::cout << "    - Node(s) and mesh(es): " << std::endl;
	for(uint32_t i = 0; i < model_.nodes_count; ++i)
	{
		tg3_node node = model_.nodes[i];
		std::cout << "        - Node " << i << ": " << std::endl;
		std::cout << "           .Mesh index: " << node.mesh << ", rotation: (" << node.rotation[0] << ", " << node.rotation[1] << ", " << node.rotation[2] << ", " << node.rotation[3] <<
			"), scale: (" << node.scale[0] << ", " << node.scale[1] << ", " << node.scale[2] <<
			"), translation: (" << node.translation[0] << ", " << node.translation[1] << ", " << node.translation[2] << ")\n";

		if(node.mesh != -1)
		{
			tg3_mesh mesh = model_.meshes[node.mesh];
			std::cout << "        - Mesh " << model_.nodes->mesh << ": " << std::endl;
			for(uint32_t j = 0; j < mesh.primitives_count; ++j)
			{
				tg3_primitive primitive = mesh.primitives[j];
				for(uint32_t k = 0; k < primitive.attributes_count; k++)
				{
					std::cout << "           .Primitive: " << primitive.attributes[k].key.data << " = " << primitive.attributes[k].value << std::endl;
				}
				std::cout << "           .Indices: " << primitive.indices << std::endl;
			}
		}
	}

	std::cout << "    - Accessor(s): " << std::endl;
	for(uint32_t i = 0; i < model_.accessors_count; ++i)
	{
		tg3_accessor accessor = model_.accessors[i];
		std::cout << "        - Accessor " << i << ": " << std::endl;
		std::cout << "           .Buffer view (index = " << accessor.buffer_view << ", offset = " << accessor.byte_offset << ")\n";
		std::cout << "           .Type = " << gltf::get_type_str(accessor.type) << ", count = " << accessor.count << ", component type = " << gltf::get_component_type_str(accessor.component_type) << std::endl;
	}

	std::cout << "    - Buffer view(s): " << std::endl;
	for(uint32_t i = 0; i < model_.buffer_views_count; ++i)
	{
		tg3_buffer_view buffer_view = model_.buffer_views[i];
		std::cout << "        - Buffer view " << i << ": " << std::endl;
		std::cout << "           .Buffer indice = " << buffer_view.buffer << std::endl;
		std::cout << "           .Length = " << buffer_view.byte_length << ", offset = " << buffer_view.byte_offset << ", target = " << gltf::get_target_str(buffer_view.target) << std::endl;
	}

	std::cout << "    - Buffer(s): " << std::endl;
	for(uint32_t i = 0; i < model_.buffers_count; ++i)
	{
		tg3_buffer buffer = model_.buffers[i];
		std::cout << "        - Buffer " << i << ": " << std::endl;
		std::cout << "           .Length = " << buffer.data.count << std::endl;
	}

	std::cout << "    - Texture(s): " << std::endl;
	for(uint32_t i = 0; i < model_.textures_count; ++i)
	{
		tg3_texture texture = model_.textures[i];
		std::cout << "        - Texture " << i << ": " << std::endl;
		std::cout << "           .Source = " << texture.source << ", sampler = " << texture.sampler << std::endl;
	}

	std::cout << "    - Sampler(s): " << std::endl;
	for(uint32_t i = 0; i < model_.samplers_count; ++i)
	{
		tg3_sampler sampler = model_.samplers[i];
		std::cout << "        - Sampler " << i << ": " << std::endl;
		std::cout << "           .MagFilter = " << gltf::get_filter_str(sampler.mag_filter) << ", minFilter = " << gltf::get_filter_str(sampler.min_filter) << ", wrapS = " << gltf::get_wrap_str(sampler.wrap_s) << ", wrapT = " << gltf::get_wrap_str(sampler.wrap_t) << std::endl;
	}

	std::cout << "    - Image(s): " << std::endl;
	for(uint32_t i = 0; i < model_.images_count; ++i)
	{
		tg3_image image = model_.images[i];
		std::cout << "        - Image " << i << ": " << std::endl;
		if(image.uri.len > 0)
		{
			std::cout << "           .URI " << "(Data URI: " << std::boolalpha << bool(tg3_is_data_uri(image.uri.data, image.uri.len)) << std::noboolalpha << ") = " << image.uri.data << std::endl;
		}
		else if(image.buffer_view != -1)
		{
			std::cout << "           .Buffer view = " << image.buffer_view << std::endl;
			std::cout << "           .Mime type = " << image.mime_type.data << std::endl;
		}
	}

	std::cout << "*********************************************************************************************\n\n";
}

/*tg3_model glTFFile::get_model() const
{
	return model_;
}*/

glm::vec3 glTFFile::get_node_position(const tg3_node& node) const
{
	glm::vec3 position(1.0f);
	if(node.has_matrix)
	{
		//TODO : à vérifier
		position = glm::vec3(node.matrix[3], node.matrix[7], node.matrix[11]);
	}
	else
	{
		position = glm::vec3(node.translation[0], node.translation[1], node.translation[2]);
	}
	return position;
}

glm::vec4 glTFFile::get_node_rotation(const tg3_node& node) const
{
	glm::vec4 rotation(0.0f);
	if(node.has_matrix)
	{
		//TODO
		std::cerr << "(Rotation1) ************************CAS PAS ENCORE GERE************************\n";
	}
	else
	{
		std::cerr << "(Rotation2) ************************CAS PAS ENCORE GERE************************\n";
		//glm::quat rotation_quaternion = glm::quat(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
		//rotation = glm::eulerAngles(rotation_quaternion)
		//rotation = glm::vec4(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]);
	}
	return rotation;
}

glm::vec3 glTFFile::get_node_scale(const tg3_node& node) const
{
	glm::vec3 scale(1.0f);
	if(node.has_matrix)
	{
		//TODO : vérifier
		glm::decompose(gltf::get_mat4_from_1d_matrix(node.matrix), scale, glm::quat(1, 0, 0, 0), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec4(1.0f));
	}
	else
	{
		scale = glm::vec3(node.scale[0], node.scale[1], node.scale[2]);
	}
	return scale;
}

/*std::vector<std::pair<unsigned int, gltf::Node>> glTFFile::get_nodes() const
{
	std::vector<std::pair<unsigned int, gltf::Node>> nodes;
	if(model_.scenes_count > 1)
	{
		std::cerr << "(Scenes count > 1) ************************CAS PAS ENCORE GERE************************\n";
	}

	//TODO : séparer cette méthode en deux (une qui prend un root node en paramètre)
	//Pour chaque root node, créer un model <=> il y aura autant de models qu'il y a de root nodes

	tg3_scene scene = model_.scenes[0];
	for(uint32_t i = 0; i < scene.nodes_count; ++i)
	{
		int32_t root_node_index = scene.nodes[i];
		tg3_node root_node = model_.nodes[root_node_index];
		get_node(root_node, nodes);
	}

	return nodes;
}*/

/*void glTFFile::get_node(const tg3_node& node, std::vector<std::pair<unsigned int, gltf::Node>>& nodes) const
{
	gltf::Node::Transform transform;
	if(node.mesh != -1)
	{
		int32_t node_mesh_index = node.mesh;
		tg3_mesh node_mesh = model_.meshes[node_mesh_index];
	}
	if(bool(node.has_matrix))
	{
		//TODO
		std::cerr << "(Has matrix == true) ************************CAS PAS ENCORE GERE************************\n";
	}
	else
	{
		transform.position_ = get_node_position(node);
		transform.rotation_ = get_node_rotation(node);
		transform.scale_ = get_node_scale(node);
	}

	//TODO : pas bien écrit 
	std::optional<Mesh> mesh = get_mesh(node);
	nodes.push_back({0, gltf::Node(mesh.value(), transform)}); //TODO : manque le niveau du node

	for(uint32_t i = 0; i < node.children_count; ++i)
	{
		get_node(model_.nodes[node.children[i]], nodes);
	}
}*/

/*gltf::Node glTFFile::get_node2(const tg3_node& node) const
{
	gltf::Node::Transform transform;
	if(node.mesh != -1)
	{
		int32_t node_mesh_index = node.mesh;
		tg3_mesh node_mesh = model_.meshes[node_mesh_index];
	}
	if(bool(node.has_matrix))
	{
		//TODO
		std::cerr << "(Has matrix == true) ************************CAS PAS ENCORE GERE************************\n";
	}
	else
	{
		transform.position_ = get_node_position(node);
		transform.rotation_ = get_node_rotation(node);
		transform.scale_ = get_node_scale(node);
	}

	//TODO : pas bien écrit 
	std::optional<Mesh> mesh = get_mesh(node);
	if(mesh.has_value())
	{
		return gltf::Node(mesh.value(), transform); //TODO : manque le niveau du node
	}
	else
	{
		return gltf::Node(transform); //TODO : manque le niveau du node
	}
}*/

gltf::Node glTFFile::get_node_and_its_children(const tg3_node& node_tg3, glm::mat4 parent_matrix) const
{
	gltf::Node::Transform transform;
	if(node_tg3.mesh != -1)
	{
		int32_t node_mesh_index = node_tg3.mesh;
		tg3_mesh node_mesh = model_.meshes[node_mesh_index];
	}

	//TODO : créer une méthode get_transform
	if(bool(node_tg3.has_matrix))
	{
		//TODO
		std::cerr << "(Has matrix == true) ************************CAS PAS ENCORE GERE************************\n";
	}
	else
	{
		transform.position_ = get_node_position(node_tg3);
		transform.rotation_ = get_node_rotation(node_tg3);
		transform.scale_ = get_node_scale(node_tg3);
	}

	//TODO : ici, remplacer transform par un appel à get_transform()
	gltf::Node node(std::string(node_tg3.name.data), get_mesh(node_tg3), transform); //TODO : manque le niveau du node
	node.parent_matrix_ = parent_matrix;
	for(uint32_t i = 0; i < node_tg3.children_count; ++i)
	{
		node.add_child(std::make_unique<gltf::Node>(get_node_and_its_children(model_.nodes[node_tg3.children[i]], node.compute_model())));
	}
	return node;
}

std::vector<gltf::Model> glTFFile::get_models() const
{
	std::vector<gltf::Model> models;
	if(model_.scenes_count > 1)
	{
		std::cerr << "(Scenes count > 1) ************************CAS PAS ENCORE GERE************************\n";
	}

	tg3_scene scene = model_.scenes[0];
	for(uint32_t i = 0; i < scene.nodes_count; ++i) //il y a un model par root node
	{
		int32_t root_node_index = scene.nodes[i];
		tg3_node root_node_tg3 = model_.nodes[root_node_index];

		//il faut remplacer get_node2 par une méthode qui retourne l'objet de type gltf::Node avec déjà ses enfants
		//gltf::Node root_node = get_node2(root_node_tg3);

		////création de la hiérarchie de nodes (à la fin, le root node doit pointer sur ses enfants qui eux-mêmes doivent pointer sur leurs enfants etc.)
		//for(uint32_t j = 0; j < root_node_tg3.children_count; ++j)
		//{
		//	root_node.add_child(std::make_unique<gltf::Node>(get_node3(model_.nodes[root_node_tg3.children[j]])));
		//}
		//models.push_back(gltf::Model(std::make_unique<gltf::Node>(std::move(root_node)))); //bizarre le std::move ici...

		models.push_back(gltf::Model(std::make_unique<gltf::Node>(get_node_and_its_children(root_node_tg3, glm::mat4(1.0f)))));
	}
	return models;
}

//count devrait être égal pour tous les attributs donc on peut prendre le count de l'attribut [0]
uint64_t glTFFile::get_attributes_count(const tg3_primitive& primitive) const
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

Vertices glTFFile::get_vertices(const tg3_primitive& primitive) const
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
				vertices.add_color_attributes(gltf::vec3_to_vec4_colors(get_float_vec_attribute<3>(attribute)));
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

/*std::vector<Mesh> glTFFile::get_meshes(const tg3_node& node) const
{
	std::vector<Texture> textures = get_textures();
	std::vector<Mesh> meshes;

	if(node.mesh != -1)
	{
		tg3_mesh mesh = model_.meshes[node.mesh];
		for(uint32_t i = 0; i < mesh.primitives_count; ++i)
		{
			tg3_primitive primitive = mesh.primitives[i];
			std::vector<GLushort> ebo_values = get_ebo_values(primitive);
			Vertices vertices = get_vertices(primitive);
			meshes.push_back(Mesh(ebo_values, vertices, textures, primitive.mode));
		}
	}
	return meshes;
}*/

std::optional<Mesh> glTFFile::get_mesh(const tg3_node& node) const
{
	if(node.mesh != -1)
	{
		std::vector<Texture> textures = get_textures();
		tg3_mesh mesh_tg3 = model_.meshes[node.mesh];

		if(mesh_tg3.primitives_count > 1)
		{
			std::cerr << "(Primitives count > 1) ************************CAS PAS ENCORE GERE************************\n";
		}

		tg3_primitive primitive = mesh_tg3.primitives[0];
		std::vector<GLushort> ebo_values = get_ebo_values(primitive);
		Vertices vertices = get_vertices(primitive);
		return Mesh(ebo_values, vertices, textures, primitive.mode);
	}
	return std::nullopt; //cas où le node ne possède pas de mesh
}

std::vector<Texture> glTFFile::get_textures() const
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
				std::cout << "****ERROR****: Embedded glTF not handled for now!\n";
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
	return textures;
}

std::vector<GLushort> glTFFile::get_ebo_values(const tg3_primitive& primitive) const
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

std::vector<glm::vec4> glTFFile::get_vec4_color_attribute(const tg3_str_int_pair& attribute) const
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
				glm::vec4 vec4(0.0f, 0.0f, 0.0f, 0.0f);
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
				glm::vec4 vec4(0.0f, 0.0f, 0.0f, 0.0f);
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