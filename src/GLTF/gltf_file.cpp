#include "gltf_file.h"
#include "gltf.h"
#include "scene.h"

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>

namespace
{
glm::vec3 get_scale_from_model_matrix(glm::mat4 model_matrix);
glm::quat get_rotation_from_model_matrix(glm::mat4 model_matrix);
glm::vec3 get_translation_from_model_matrix(glm::mat4 model_matrix);
glm::vec3 get_skew_from_model_matrix(glm::mat4 model_matrix);
glm::vec4 get_perspective_from_model_matrix(glm::mat4 model_matrix);
glm::vec3 get_node_position(const tg3_node& node_tg3);
glm::quat get_node_rotation(const tg3_node& node_tg3);
glm::vec3 get_node_scale(const tg3_node& node_tg3);
gltf::Node get_node(const tg3_model& model_tg3, const tg3_node& node_tg3, glm::mat4 parent_matrix);
uint64_t get_attributes_count(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
Vertices get_vertices(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
Transform get_transform(const tg3_node& node_tg3);
std::optional<Mesh> get_mesh(const tg3_model& model_tg3, const tg3_node& node_tg3);
std::vector<Texture> get_textures(const tg3_model& model_tg3);
std::vector<GLushort> get_ebo_values(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
std::vector<glm::vec4> get_vec4_color_attribute(const tg3_model& model_tg3, const tg3_str_int_pair& attribute_tg3);

}

namespace gltf
{

glTFFile::glTFFile(std::string_view path)
	: path_(path), gltf_kind_(get_glTFKind(path))
{
	open();
	print_info();
}

glTFFile::~glTFFile()
{
	close();
}

void glTFFile::open()
{
	tg3_parse_options options_tg3;
	tg3_parse_options_init(&options_tg3);
	tg3_error_stack_init(&error_stack_tg3_);

	if(tg3_parse_file(&model_tg3_, &error_stack_tg3_, path_.c_str(), uint32_t(path_.length()), &options_tg3) != TG3_OK)
	{
		for(uint32_t i = 0; i < error_stack_tg3_.count; i++)
		{
			tg3_error_entry error_entry_tg3 = error_stack_tg3_.entries[i];
			std::string str = error_entry_tg3.message ? error_entry_tg3.message : "(null)";
			std::cout << "**TG3 Error** => severity: " << int(error_entry_tg3.severity) << ", message: " << str << std::endl;

			if(error_entry_tg3.code == TG3_ERR_FILE_NOT_FOUND || error_entry_tg3.code == TG3_ERR_FILE_READ)
			{
				std::cout << "Exiting the program because the file \"" << path_ << "\" doesn't exist or couldn't be read!\n";
				exit(EXIT_FAILURE);
			}
		}
	}
}

void glTFFile::close()
{
	tg3_error_stack_free(&error_stack_tg3_);
	tg3_model_free(&model_tg3_);
}

//TODO : utile ??
glTFFile::glTFKind glTFFile::get_glTFKind(std::string_view path) const
{
	if(path.find("resources/models/") != std::string_view::npos)
	{
		return glTFKind::MODEL;
	}
	else if(path.find("resources/maps/") != std::string_view::npos)
	{
		return glTFKind::MAP;
	}
	else
	{
		return glTFKind::UNKNOWN;
	}
}

void glTFFile::print_info() const
{
	std::cout << "\n**** Info on the glTF file ****\n";
	std::cout << "- General information: " << std::endl;
	std::cout << "    - File name: " << path_ << std::endl;
	std::cout << "    - The file contains " << model_tg3_.scenes->nodes_count << " root node(s).\n";
	std::cout << "    - There are " << model_tg3_.scenes_count << " scene(s), " << model_tg3_.nodes_count << " node(s), " << model_tg3_.meshes_count << " mesh(es), " << model_tg3_.accessors_count << " accessor(s), " << model_tg3_.buffer_views_count << " buffer view(s), " << model_tg3_.buffers_count << " buffer(s), " << model_tg3_.textures_count << " texture(s), " << model_tg3_.samplers_count << " sampler(s) and " << model_tg3_.images_count << " image(s).\n";

	std::cout << "- File content: " << std::endl;
	std::cout << "    - Node(s) and mesh(es): " << std::endl;
	for(uint32_t i = 0; i < model_tg3_.nodes_count; ++i)
	{
		tg3_node node_tg3 = model_tg3_.nodes[i];
		std::cout << "        - Node " << i << ": " << std::endl;

		if(node_tg3.ext.extras != nullptr)
		{
			std::cout << "           .Extras: <\"" << node_tg3.ext.extras->object_data->key.data << "\" : \"" << node_tg3.ext.extras->object_data->value.string_val.data << "\">" << std::endl;
		}

		std::cout << "           .Mesh index: " << node_tg3.mesh << ", rotation: (" << node_tg3.rotation[0] << ", " << node_tg3.rotation[1] << ", " << node_tg3.rotation[2] << ", " << node_tg3.rotation[3] <<
			"), scale: (" << node_tg3.scale[0] << ", " << node_tg3.scale[1] << ", " << node_tg3.scale[2] <<
			"), translation: (" << node_tg3.translation[0] << ", " << node_tg3.translation[1] << ", " << node_tg3.translation[2] << ")\n";

		if(node_tg3.mesh != -1)
		{
			tg3_mesh mesh_tg3 = model_tg3_.meshes[node_tg3.mesh];
			std::cout << "        - Mesh " << model_tg3_.nodes->mesh << ": " << std::endl;
			for(uint32_t j = 0; j < mesh_tg3.primitives_count; ++j)
			{
				tg3_primitive primitive_tg3 = mesh_tg3.primitives[j];
				for(uint32_t k = 0; k < primitive_tg3.attributes_count; k++)
				{
					std::cout << "           .Primitive: " << primitive_tg3.attributes[k].key.data << " = " << primitive_tg3.attributes[k].value << std::endl;
				}
				std::cout << "           .Indices: " << primitive_tg3.indices << std::endl;
			}
		}
	}

	std::cout << "    - Accessor(s): " << std::endl;
	for(uint32_t i = 0; i < model_tg3_.accessors_count; ++i)
	{
		tg3_accessor accessor_tg3 = model_tg3_.accessors[i];
		std::cout << "        - Accessor " << i << ": " << std::endl;
		std::cout << "           .Buffer view (index = " << accessor_tg3.buffer_view << ", offset = " << accessor_tg3.byte_offset << ")\n";
		std::cout << "           .Type = " << gltf::get_type_str(accessor_tg3.type) << ", count = " << accessor_tg3.count << ", component type = " << gltf::get_component_type_str(accessor_tg3.component_type) << std::endl;
	}

	std::cout << "    - Buffer view(s): " << std::endl;
	for(uint32_t i = 0; i < model_tg3_.buffer_views_count; ++i)
	{
		tg3_buffer_view buffer_view_tg3 = model_tg3_.buffer_views[i];
		std::cout << "        - Buffer view " << i << ": " << std::endl;
		std::cout << "           .Buffer indice = " << buffer_view_tg3.buffer << std::endl;
		std::cout << "           .Length = " << buffer_view_tg3.byte_length << ", offset = " << buffer_view_tg3.byte_offset << ", target = " << gltf::get_target_str(buffer_view_tg3.target) << std::endl;
	}

	std::cout << "    - Buffer(s): " << std::endl;
	for(uint32_t i = 0; i < model_tg3_.buffers_count; ++i)
	{
		tg3_buffer buffer_tg3 = model_tg3_.buffers[i];
		std::cout << "        - Buffer " << i << ": " << std::endl;
		std::cout << "           .Length = " << buffer_tg3.data.count << std::endl;
	}

	std::cout << "    - Texture(s): " << std::endl;
	for(uint32_t i = 0; i < model_tg3_.textures_count; ++i)
	{
		tg3_texture texture_tg3 = model_tg3_.textures[i];
		std::cout << "        - Texture " << i << ": " << std::endl;
		std::cout << "           .Source = " << texture_tg3.source << ", sampler = " << texture_tg3.sampler << std::endl;
	}

	std::cout << "    - Sampler(s): " << std::endl;
	for(uint32_t i = 0; i < model_tg3_.samplers_count; ++i)
	{
		tg3_sampler sampler_tg3 = model_tg3_.samplers[i];
		std::cout << "        - Sampler " << i << ": " << std::endl;
		std::cout << "           .MagFilter = " << gltf::get_filter_str(sampler_tg3.mag_filter) << ", minFilter = " << gltf::get_filter_str(sampler_tg3.min_filter)
			<< ", wrapS = " << gltf::get_wrap_str(sampler_tg3.wrap_s) << ", wrapT = " << gltf::get_wrap_str(sampler_tg3.wrap_t) << std::endl;
	}

	std::cout << "    - Image(s): " << std::endl;
	for(uint32_t i = 0; i < model_tg3_.images_count; ++i)
	{
		tg3_image image_tg3 = model_tg3_.images[i];
		std::cout << "        - Image " << i << ": " << std::endl;
		if(image_tg3.uri.len > 0)
		{
			std::cout << "           .URI " << "(Data URI: " << std::boolalpha << bool(tg3_is_data_uri(image_tg3.uri.data, image_tg3.uri.len)) << std::noboolalpha << ") = " << image_tg3.uri.data << std::endl;
		}
		else if(image_tg3.buffer_view != -1)
		{
			std::cout << "           .Buffer view = " << image_tg3.buffer_view << std::endl;
			std::cout << "           .Mime type = " << image_tg3.mime_type.data << std::endl;
		}
	}

	std::cout << "*********************************************************************************************\n\n";
}

std::unique_ptr<Scene> glTFFile::get_scene() const
{
	std::vector<std::unique_ptr<Node>> root_nodes;
	if(model_tg3_.scenes_count > 1)
	{
		std::cerr << "(Scenes count > 1) ************************CAS PAS ENCORE GERE************************\n";
	}

	tg3_scene scene_tg3 = model_tg3_.scenes[0];
	for(uint32_t i = 0; i < scene_tg3.nodes_count; ++i)
	{
		int32_t root_node_index = scene_tg3.nodes[i];
		tg3_node root_node_tg3 = model_tg3_.nodes[root_node_index];
		root_nodes.push_back(std::make_unique<Node>(get_node(model_tg3_, root_node_tg3, glm::mat4(1.0f))));
	}
	return std::make_unique<Scene>(std::move(root_nodes), gltf_kind_ == glTFKind::MODEL ? true : false);
}

}

namespace
{

glm::vec3 get_scale_from_model_matrix(glm::mat4 model_matrix)
{
	glm::vec3 scale(1.0f);
	glm::quat orientation;
	glm::vec3 translation(0.0f);
	glm::vec3 skew(0.0f);
	glm::vec4 perspective(0.0f);
	glm::decompose(model_matrix, scale, orientation, translation, skew, perspective);
	return scale;
}

glm::quat get_rotation_from_model_matrix(glm::mat4 model_matrix)
{
	glm::vec3 scale(1.0f);
	glm::quat orientation;
	glm::vec3 translation(0.0f);
	glm::vec3 skew(0.0f);
	glm::vec4 perspective(0.0f);
	glm::decompose(model_matrix, scale, orientation, translation, skew, perspective);
	return orientation;
}

glm::vec3 get_translation_from_model_matrix(glm::mat4 model_matrix)
{
	glm::vec3 scale(1.0f);
	glm::quat orientation;
	glm::vec3 translation(0.0f);
	glm::vec3 skew(0.0f);
	glm::vec4 perspective(0.0f);
	glm::decompose(model_matrix, scale, orientation, translation, skew, perspective);
	return translation;
}

glm::vec3 get_skew_from_model_matrix(glm::mat4 model_matrix)
{
	glm::vec3 scale(1.0f);
	glm::quat orientation;
	glm::vec3 translation(0.0f);
	glm::vec3 skew(0.0f);
	glm::vec4 perspective(0.0f);
	glm::decompose(model_matrix, scale, orientation, translation, skew, perspective);
	return skew;
}

glm::vec4 get_perspective_from_model_matrix(glm::mat4 model_matrix)
{
	glm::vec3 scale(1.0f);
	glm::quat orientation;
	glm::vec3 translation(0.0f);
	glm::vec3 skew(0.0f);
	glm::vec4 perspective(0.0f);
	glm::decompose(model_matrix, scale, orientation, translation, skew, perspective);
	return perspective;
}

template <glm::length_t L>
std::vector<glm::vec<L, float>> get_float_vec_attribute(const tg3_model& model_tg3, const tg3_str_int_pair& attribute_tg3)
{
	using vector_type = std::vector<glm::vec<L, float>>;

	tg3_accessor accessor_tg3 = model_tg3.accessors[attribute_tg3.value];
	tg3_buffer_view buffer_view_tg3 = model_tg3.buffer_views[accessor_tg3.buffer_view];
	tg3_buffer buffer_tg3 = model_tg3.buffers[buffer_view_tg3.buffer];

	std::string component_type_str = gltf::get_component_type_str(accessor_tg3.component_type);
	std::size_t component_type_size = gltf::get_component_type_size(accessor_tg3.component_type);
	std::string type_str = gltf::get_type_str(accessor_tg3.type);

	uint64_t stride = buffer_view_tg3.byte_stride != 0 ? buffer_view_tg3.byte_stride : sizeof(vector_type::value_type);
	vector_type vector;

	for(uint64_t i = buffer_view_tg3.byte_offset + accessor_tg3.byte_offset; i < buffer_view_tg3.byte_offset + buffer_view_tg3.byte_length; i += stride)
	{
		vector_type::value_type vector_element(0.0f);
		for(uint64_t j = 0; j < component_type_size * vector_type::value_type::length(); j += component_type_size)
		{
			uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 bits (tous les attributs contiennent des composants de type GL_FLOAT)
			for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
			{
				attribute_ieee754 |= uint32_t(buffer_tg3.data.data[i + j + k] << k * 8); //8 pour convertir les octets en bits
			}
			GLfloat attribute_float = utils::ieee754_to_float(attribute_ieee754);
			uint64_t component = j / component_type_size;
			vector_element[component % L] = attribute_float;
		}
		vector.push_back(vector_element);
	}
	return vector;
}

glm::vec3 get_node_position(const tg3_node& node_tg3)
{
	glm::vec3 position(1.0f);
	if(node_tg3.has_matrix)
	{
		//TODO : à vérifier
		position = glm::vec3(node_tg3.matrix[3], node_tg3.matrix[7], node_tg3.matrix[11]);
	}
	else
	{
		position = glm::vec3(node_tg3.translation[0], node_tg3.translation[1], node_tg3.translation[2]);
	}
	return position;
}

glm::quat get_node_rotation(const tg3_node& node_tg3)
{
	glm::quat rotation_quaternion;
	if(node_tg3.has_matrix)
	{
		//TODO
		std::cerr << "(Rotation1) ************************CAS PAS ENCORE GERE************************\n";
		rotation_quaternion = get_rotation_from_model_matrix(gltf::get_mat4_from_1d_matrix(node_tg3.matrix));
	}
	else
	{
		rotation_quaternion = glm::quat(float(node_tg3.rotation[3]), float(node_tg3.rotation[0]), float(node_tg3.rotation[1]), float(node_tg3.rotation[2]));
	}
	//std::cout << node_tg3.name.data <<  ", Quaternion: " << rotation_quaternion.x << ", " << rotation_quaternion.y << ", " << rotation_quaternion.z << ", " << rotation_quaternion.w << std::endl;
	return rotation_quaternion;
}

glm::vec3 get_node_scale(const tg3_node& node_tg3)
{
	glm::vec3 scale(1.0f);
	if(node_tg3.has_matrix)
	{
		//TODO : vérifier
		scale = get_scale_from_model_matrix(gltf::get_mat4_from_1d_matrix(node_tg3.matrix));
	}
	else
	{
		scale = glm::vec3(node_tg3.scale[0], node_tg3.scale[1], node_tg3.scale[2]);
	}
	return scale;
}

gltf::Node get_node(const tg3_model& model_tg3, const tg3_node& node_tg3, glm::mat4 parent_matrix)
{
	gltf::Node node(std::string(node_tg3.name.data), get_transform(node_tg3), get_mesh(model_tg3, node_tg3));
	node.parent_matrix_ = parent_matrix;
	for(uint32_t i = 0; i < node_tg3.children_count; ++i)
	{
		node.add_child(std::make_unique<gltf::Node>(get_node(model_tg3, model_tg3.nodes[node_tg3.children[i]], node.compute_model())));
	}
	return node;
}

//count devrait être égal pour tous les attributs donc on peut prendre le count de l'attribut [0]
uint64_t get_attributes_count(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3)
{
	if(primitive_tg3.attributes_count > 0)
	{
		return model_tg3.accessors[primitive_tg3.attributes[0].value].count;
	}
	else
	{
		return 0;
	}
}

Vertices get_vertices(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3)
{
	Vertices vertices(get_attributes_count(model_tg3, primitive_tg3));

	for(uint32_t i = 0; i < primitive_tg3.attributes_count; i++)
	{
		tg3_str_int_pair attribute_tg3 = primitive_tg3.attributes[i];
		std::string attribute_name_str = std::string(attribute_tg3.key.data);
		tg3_accessor accessor_tg3 = model_tg3.accessors[attribute_tg3.value];

		if(attribute_name_str == "POSITION") //vec3 de float
		{
			vertices.add_position_attributes(get_float_vec_attribute<3>(model_tg3, attribute_tg3));
		}
		else if(attribute_name_str == "NORMAL") //vec3 de float
		{
			vertices.add_normal_attributes(get_float_vec_attribute<3>(model_tg3, attribute_tg3));
		}
		else if(attribute_name_str.find("TEXCOORD_") != std::string::npos) //vec2
		{
			vertices.add_texture_coordinates_attributes(get_float_vec_attribute<2>(model_tg3, attribute_tg3));
		}
		else if(attribute_name_str.find("COLOR_") != std::string::npos) //vec3 ou vec4
		{
			if(gltf::get_type_str(accessor_tg3.type) == "VEC3")
			{
				vertices.add_color_attributes(gltf::vec3_to_vec4_colors(get_float_vec_attribute<3>(model_tg3, attribute_tg3)));
			}
			else if(gltf::get_type_str(accessor_tg3.type) == "VEC4")
			{
				vertices.add_color_attributes(get_vec4_color_attribute(model_tg3, attribute_tg3));
			}
		}
		else
		{
			std::cout << "****ERROR****: Unknown attribute name!\n";
		}
	}
	return vertices;
}

Transform get_transform(const tg3_node& node_tg3)
{
	Transform transform;
	if(bool(node_tg3.has_matrix))
	{
		//TODO
		std::cerr << "(Has matrix == true) ************************CAS PAS ENCORE GERE************************\n";
	}
	else
	{
		transform.position_ = get_node_position(node_tg3);
		transform.rotation_quaternion_ = get_node_rotation(node_tg3);
		transform.scale_ = get_node_scale(node_tg3);
	}
	return transform;
}

std::optional<Mesh> get_mesh(const tg3_model& model_tg3, const tg3_node& node_tg3)
{
	if(node_tg3.mesh != -1)
	{
		std::vector<Texture> textures = get_textures(model_tg3);
		tg3_mesh mesh_tg3 = model_tg3.meshes[node_tg3.mesh];

		if(mesh_tg3.primitives_count > 1)
		{
			std::cerr << "(Primitives count > 1) ************************CAS PAS ENCORE GERE************************\n";
		}

		tg3_primitive primitive_tg3 = mesh_tg3.primitives[0];
		std::vector<GLushort> ebo_values = get_ebo_values(model_tg3, primitive_tg3);
		Vertices vertices = get_vertices(model_tg3, primitive_tg3);
		return Mesh(ebo_values, vertices, textures, primitive_tg3.mode);
	}
	return std::nullopt; //cas où le node ne possède pas de mesh
}

std::vector<Texture> get_textures(const tg3_model& model_tg3)
{
	std::vector<Texture> textures;
	for(uint32_t i = 0; i < model_tg3.textures_count; ++i)
	{
		Texture mesh_texture;
		tg3_texture texture_tg3 = model_tg3.textures[i];
		tg3_image image_tg3 = model_tg3.images[texture_tg3.source];

		mesh_texture.texture_unit_ = i;
		if(image_tg3.buffer_view == -1)
		{
			std::string image_str = std::string(image_tg3.uri.data);
			if(bool(tg3_is_data_uri(image_tg3.uri.data, image_tg3.uri.len)))
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
			tg3_buffer_view buffer_view_tg3 = model_tg3.buffer_views[image_tg3.buffer_view];
			tg3_buffer buffer_tg3 = model_tg3.buffers[buffer_view_tg3.buffer];
			mesh_texture.image_data_.reserve(buffer_view_tg3.byte_length);

			//std::copy(buffer.data.data, buffer.data.data + buffer_view.byte_length, std::back_inserter(mesh_texture.image_data_)); //TODO : mieux que la boucle for suivante ?
			for(uint64_t j = 0; j < buffer_view_tg3.byte_length; ++j)
			{
				mesh_texture.image_data_.push_back(buffer_tg3.data.data[buffer_view_tg3.byte_offset + j]);
			}
		}

		tg3_sampler sampler_tg3 = model_tg3.samplers[texture_tg3.sampler];

		mesh_texture.min_filter_ = sampler_tg3.min_filter;
		mesh_texture.mag_filter_ = sampler_tg3.mag_filter;
		mesh_texture.wrap_s_ = sampler_tg3.wrap_s;
		mesh_texture.wrap_t_ = sampler_tg3.wrap_t;

		textures.push_back(mesh_texture);
	}
	return textures;
}

std::vector<GLushort> get_ebo_values(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3)
{
	tg3_accessor accessor_tg3 = model_tg3.accessors[primitive_tg3.indices];
	tg3_buffer_view buffer_view_tg3 = model_tg3.buffer_views[accessor_tg3.buffer_view];
	tg3_buffer buffer_tg3 = model_tg3.buffers[buffer_view_tg3.buffer];

	std::vector<GLushort> ebo_value;
	ebo_value.reserve(accessor_tg3.count);

	for(uint64_t i = buffer_view_tg3.byte_offset; i < buffer_view_tg3.byte_offset + buffer_view_tg3.byte_length; i += sizeof(GLushort))
	{
		GLushort indice_value = (buffer_tg3.data.data[i + 1] << 8) | buffer_tg3.data.data[i]; //=> little-endian
		ebo_value.push_back(indice_value);
	}
	return ebo_value;
}

std::vector<glm::vec4> get_vec4_color_attribute(const tg3_model& model_tg3, const tg3_str_int_pair& attribute_tg3)
{
	tg3_accessor accessor_tg3 = model_tg3.accessors[attribute_tg3.value];
	std::string component_type_str = gltf::get_component_type_str(accessor_tg3.component_type);
	std::vector<glm::vec4> vec4_vector;

	if(component_type_str == "GL_FLOAT")
	{
		return get_float_vec_attribute<4>(model_tg3, attribute_tg3);
	}
	else
	{
		tg3_buffer_view buffer_view_tg3 = model_tg3.buffer_views[accessor_tg3.buffer_view];
		tg3_buffer buffer_tg3 = model_tg3.buffers[buffer_view_tg3.buffer];
		std::size_t component_type_size = gltf::get_component_type_size(accessor_tg3.component_type);

		if(component_type_str == "GL_UNSIGNED_BYTE")
		{
			for(uint64_t i = buffer_view_tg3.byte_offset + accessor_tg3.byte_offset; i < buffer_view_tg3.byte_offset + buffer_view_tg3.byte_length; i += 4 * sizeof(GLubyte)) //car sizeof(glm::vec4) != 4 * sizeof(sizeof(GLubyte))
			{
				glm::vec4 vec4(0.0f, 0.0f, 0.0f, 0.0f);
				for(uint64_t j = 0; j < component_type_size * glm::vec4::length(); j += component_type_size)
				{
					uint8_t attribute_u8 = 0; //uint8_t car un unsigned short fait 8 bits
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_u8 |= uint8_t(buffer_tg3.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
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
			for(uint64_t i = buffer_view_tg3.byte_offset + accessor_tg3.byte_offset; i < buffer_view_tg3.byte_offset + buffer_view_tg3.byte_length; i += 4 * sizeof(GLushort)) //car sizeof(glm::vec4) != 4 * sizeof(sizeof(GLushort))
			{
				glm::vec4 vec4(0.0f, 0.0f, 0.0f, 0.0f);
				for(uint64_t j = 0; j < component_type_size * glm::vec4::length(); j += component_type_size)
				{
					uint16_t attribute_u16 = 0; //uint16_t car un unsigned short fait 16 bits
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_u16 |= uint16_t(buffer_tg3.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
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


}