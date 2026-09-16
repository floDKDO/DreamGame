#include "gltf_file.h"
#include "gltf.h"
#include "node.h"
#include "aabb.h"
#include "Common/utils.h"
#include "Logging/logging.h"
#include "gl_resource_manager.h"

#include <iostream>

namespace
{

glm::vec3 get_node_position(const tg3_node& node_tg3);
glm::quat get_node_rotation(const tg3_node& node_tg3);
glm::vec3 get_node_scale(const tg3_node& node_tg3);
gltf::Node get_node(std::string_view path, const tg3_model& model_tg3, const tg3_node& node_tg3, glm::mat4 parent_matrix);
uint64_t get_attributes_count(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
Vertices get_vertices(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
Vertices get_aabb_vertices(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
Transform get_transform(const tg3_node& node_tg3);
resource::MeshKey get_mesh(std::string_view path, const tg3_model& model_tg3, const tg3_node& node_tg3);
std::optional<tg3_accessor> get_accessor_from_attribute(std::string_view attribute, const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
glm::vec3 get_min_values(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
glm::vec3 get_max_values(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
std::optional<AABB> get_aabb(const tg3_model& model_tg3, const tg3_node& node_tg3);
std::vector<std::string> get_textures(const tg3_model& model_tg3);
std::vector<GLushort> get_ebo_values(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3);
std::vector<glm::vec4> get_vec4_color_attribute(const tg3_model& model_tg3, const tg3_str_int_pair& attribute_tg3);
bool has_textures(const tg3_model& model_tg3);

}

namespace gltf
{

glTFFile::glTFFile(std::string_view path)
	: path_(path)
{
	open();
	//print_info(); //TODO
}

glTFFile::~glTFFile()
{
	close();
}

void glTFFile::open()
{
	if(path_.find("resources/models/") == std::string::npos)
	{
		logging::log("This glTF file is not a model. Exiting!", logging::Severity::CRITICAL);
		exit(EXIT_FAILURE);
	}

	tg3_parse_options options_tg3;
	tg3_parse_options_init(&options_tg3);
	tg3_error_stack_init(&error_stack_tg3_);

	if(tg3_parse_file(&model_tg3_, &error_stack_tg3_, path_.c_str(), uint32_t(path_.length()), &options_tg3) != TG3_OK)
	{
		for(uint32_t i = 0; i < error_stack_tg3_.count; i++)
		{
			tg3_error_entry error_entry_tg3 = error_stack_tg3_.entries[i];
			std::string str = error_entry_tg3.message ? error_entry_tg3.message : "(null)";
			logging::log("**TG3 Error** => severity: " + std::to_string(int(error_entry_tg3.severity)) + ", message: " + str, logging::Severity::CRITICAL);

			if(error_entry_tg3.code == TG3_ERR_FILE_NOT_FOUND || error_entry_tg3.code == TG3_ERR_FILE_READ)
			{
				logging::log("Exiting the program because the file \"" + path_ + "\" does not exist or could not be read!", logging::Severity::CRITICAL);
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

void glTFFile::print_info() const
{
	std::cout << std::endl;
	logging::log("**** Info on the glTF file ****", logging::Severity::DEBUG);
	logging::log("- General information: ", logging::Severity::DEBUG);

	logging::log("    - File name: " + path_, logging::Severity::DEBUG);
	logging::log("    - The file contains " + std::to_string(model_tg3_.scenes->nodes_count) + " root node(s).", logging::Severity::DEBUG);
	logging::log("    - There are " + std::to_string(model_tg3_.scenes_count) + " scene(s), " + std::to_string(model_tg3_.nodes_count) + " node(s), "
		+ std::to_string(model_tg3_.meshes_count) + " mesh(es), " + std::to_string(model_tg3_.accessors_count) + " accessor(s), " 
		+ std::to_string(model_tg3_.buffer_views_count) + " buffer view(s), " + std::to_string(model_tg3_.buffers_count) + " buffer(s), "
		+ std::to_string(model_tg3_.textures_count) + " texture(s), " + std::to_string(model_tg3_.samplers_count) + " sampler(s) and " + std::to_string(model_tg3_.images_count) + " image(s).", logging::Severity::DEBUG);

	logging::log("- File content: ", logging::Severity::DEBUG);
	logging::log("    - Node(s) and mesh(es): ", logging::Severity::DEBUG);
	for(uint32_t i = 0; i < model_tg3_.nodes_count; ++i)
	{
		tg3_node node_tg3 = model_tg3_.nodes[i];
		logging::log("        - Node " + std::to_string(i) + ": ", logging::Severity::DEBUG);

		if(node_tg3.ext.extras != nullptr)
		{
			const tg3_kv_pair* extras_tg3 = node_tg3.ext.extras->object_data;
			if(extras_tg3->key.len > 0 && extras_tg3->value.string_val.len > 0)
			{
				logging::log("           .Extras: <\"" + std::string(extras_tg3->key.data) + "\": \"" + std::string(extras_tg3->value.string_val.data) + "\">", logging::Severity::DEBUG);
			}
		}

		logging::log("           .Mesh index: " + std::to_string(node_tg3.mesh) + ", rotation: (" + std::to_string(node_tg3.rotation[0]) + ", " + std::to_string(node_tg3.rotation[1]) 
			+ ", " + std::to_string(node_tg3.rotation[2]) + ", " + std::to_string(node_tg3.rotation[3]) + "), scale: (" + std::to_string(node_tg3.scale[0]) + ", " + std::to_string(node_tg3.scale[1])
			+ ", " + std::to_string(node_tg3.scale[2]) + "), translation: (" + std::to_string(node_tg3.translation[0]) + ", " + std::to_string(node_tg3.translation[1])
			+ ", " + std::to_string(node_tg3.translation[2]) + ")", logging::Severity::DEBUG);

		if(node_tg3.mesh != -1)
		{
			tg3_mesh mesh_tg3 = model_tg3_.meshes[node_tg3.mesh];
			logging::log("        - Mesh " + std::to_string(model_tg3_.nodes->mesh) + ": ", logging::Severity::DEBUG);
			for(uint32_t j = 0; j < mesh_tg3.primitives_count; ++j)
			{
				tg3_primitive primitive_tg3 = mesh_tg3.primitives[j];
				for(uint32_t k = 0; k < primitive_tg3.attributes_count; k++)
				{
					const tg3_str_int_pair attribute_tg3 = primitive_tg3.attributes[k];
					if(attribute_tg3.key.len > 0)
					{
						logging::log("           .Primitive: " + std::string(attribute_tg3.key.data) + " = " + std::to_string(attribute_tg3.value), logging::Severity::DEBUG);
					}
				}
				logging::log("           .Indices: " + std::to_string(primitive_tg3.indices), logging::Severity::DEBUG);
			}
		}
	}

	logging::log("    - Accessor(s): ", logging::Severity::DEBUG);
	for(uint32_t i = 0; i < model_tg3_.accessors_count; ++i)
	{
		tg3_accessor accessor_tg3 = model_tg3_.accessors[i];
		logging::log("        - Accessor " + std::to_string(i) + ": ", logging::Severity::DEBUG);
		logging::log("           .Buffer view (index = " + std::to_string(accessor_tg3.buffer_view) + ", offset = " + std::to_string(accessor_tg3.byte_offset) + ")", logging::Severity::DEBUG);
		logging::log("           .Type = " + gltf::get_type_str(accessor_tg3.type) + ", count = " + std::to_string(accessor_tg3.count) + ", component type = " 
			+ gltf::get_component_type_str(accessor_tg3.component_type), logging::Severity::DEBUG);
	}

	logging::log("    - Buffer view(s): ", logging::Severity::DEBUG);
	for(uint32_t i = 0; i < model_tg3_.buffer_views_count; ++i)
	{
		tg3_buffer_view buffer_view_tg3 = model_tg3_.buffer_views[i];
		logging::log("        - Buffer view " + std::to_string(i) + ": ", logging::Severity::DEBUG);
		logging::log("           .Buffer indice = " + std::to_string(buffer_view_tg3.buffer), logging::Severity::DEBUG);
		logging::log("           .Length = " + std::to_string(buffer_view_tg3.byte_length) + ", offset = " + std::to_string(buffer_view_tg3.byte_offset) + ", target = " 
			+ gltf::get_target_str(buffer_view_tg3.target), logging::Severity::DEBUG);
	}

	logging::log("    - Buffer(s): ", logging::Severity::DEBUG);
	for(uint32_t i = 0; i < model_tg3_.buffers_count; ++i)
	{
		tg3_buffer buffer_tg3 = model_tg3_.buffers[i];
		logging::log("        - Buffer " + std::to_string(i) + ": ", logging::Severity::DEBUG);
		logging::log("           .Length = " + std::to_string(buffer_tg3.data.count), logging::Severity::DEBUG);
	}

	logging::log("    - Texture(s): ", logging::Severity::DEBUG);
	for(uint32_t i = 0; i < model_tg3_.textures_count; ++i)
	{
		tg3_texture texture_tg3 = model_tg3_.textures[i];
		logging::log("        - Texture " + std::to_string(i) + ": ", logging::Severity::DEBUG);
		logging::log("           .Source = " + std::to_string(texture_tg3.source) + ", sampler = " + std::to_string(texture_tg3.sampler), logging::Severity::DEBUG);
	}

	logging::log("    - Sampler(s): ", logging::Severity::DEBUG);
	for(uint32_t i = 0; i < model_tg3_.samplers_count; ++i)
	{
		tg3_sampler sampler_tg3 = model_tg3_.samplers[i];
		logging::log("        - Sampler " + std::to_string(i) + ": ", logging::Severity::DEBUG);
		logging::log("           .MagFilter = " + gltf::get_filter_str(sampler_tg3.mag_filter) + ", minFilter = " + gltf::get_filter_str(sampler_tg3.min_filter)
			+ ", wrapS = " + gltf::get_wrap_str(sampler_tg3.wrap_s) + ", wrapT = " + gltf::get_wrap_str(sampler_tg3.wrap_t), logging::Severity::DEBUG);
	}

	logging::log("    - Image(s): ", logging::Severity::DEBUG);
	for(uint32_t i = 0; i < model_tg3_.images_count; ++i)
	{
		tg3_image image_tg3 = model_tg3_.images[i];
		logging::log("        - Image " + std::to_string(i) + ": ", logging::Severity::DEBUG);
		if(image_tg3.uri.len > 0)
		{
			logging::log("           .URI (Data URI: " + utils::get_string_from_bool(bool(tg3_is_data_uri(image_tg3.uri.data, image_tg3.uri.len))) + ") = " + std::string(image_tg3.uri.data), logging::Severity::DEBUG);
		}
		else if(image_tg3.buffer_view != -1)
		{
			logging::log("           .Buffer view = " + std::to_string(image_tg3.buffer_view) + ": ", logging::Severity::DEBUG);
			if(image_tg3.mime_type.len > 0)
			{
				logging::log("           .Mime type = " + std::string(image_tg3.mime_type.data) + ": ", logging::Severity::DEBUG);
			}
		}
	}

	logging::log("*********************************************************************************************", logging::Severity::DEBUG);
	std::cout << std::endl;
	std::cout << std::endl;
}

Node glTFFile::get_root_node() const
{
	if(model_tg3_.scenes_count > 1)
	{
		logging::log("Number of scenes > 1 not handled", logging::Severity::WARNING);
	}

	tg3_scene scene_tg3 = model_tg3_.scenes[0];
	int32_t root_node_index = scene_tg3.nodes[0];
	tg3_node root_node_tg3 = model_tg3_.nodes[root_node_index];

	if(root_node_tg3.children_count > 1 && root_node_tg3.mesh == -1)
	{
		
	}

	return get_node(path_, model_tg3_, root_node_tg3, glm::mat4(1.0f)); //les root nodes n'ont pas de parent donc ont une matrice identitée pour leur parent_matrix
}

}

namespace
{

template <glm::length_t L>
std::vector<glm::vec<L, float>> get_float_vec_attribute(const tg3_model& model_tg3, const tg3_str_int_pair& attribute_tg3)
{
	using vector_type = std::vector<glm::vec<L, float>>;

	tg3_accessor accessor_tg3 = model_tg3.accessors[attribute_tg3.value];
	tg3_buffer_view buffer_view_tg3 = model_tg3.buffer_views[accessor_tg3.buffer_view];
	tg3_buffer buffer_tg3 = model_tg3.buffers[buffer_view_tg3.buffer];

	std::size_t component_type_size = gltf::get_component_type_size(accessor_tg3.component_type);

	uint64_t stride = buffer_view_tg3.byte_stride != 0 ? buffer_view_tg3.byte_stride : sizeof(vector_type::value_type);
	vector_type attribute_vector;

	for(uint64_t i = buffer_view_tg3.byte_offset + accessor_tg3.byte_offset; i < buffer_view_tg3.byte_offset + buffer_view_tg3.byte_length; i += stride)
	{
		vector_type::value_type attribute(0.0f);
		for(uint64_t j = 0; j < component_type_size * vector_type::value_type::length(); j += component_type_size)
		{
			uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 bits (tous les attributs contiennent des composants de type GL_FLOAT)
			for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
			{
				attribute_ieee754 |= uint32_t(buffer_tg3.data.data[i + j + k] << k * 8); //8 pour convertir les octets en bits
			}
			GLfloat attribute_float = utils::ieee754_to_float(attribute_ieee754);
			uint64_t component = j / component_type_size;
			attribute[component % L] = attribute_float;
		}
		attribute_vector.push_back(attribute);
	}
	return attribute_vector;
}

glm::vec3 get_node_position(const tg3_node& node_tg3)
{
	if(node_tg3.has_matrix)
	{
		return utils::get_translation_from_model_matrix(gltf::get_mat4_from_1d_matrix(node_tg3.matrix));
	}
	else
	{
		return glm::vec3(node_tg3.translation[0], node_tg3.translation[1], node_tg3.translation[2]);
	}
}

glm::quat get_node_rotation(const tg3_node& node_tg3)
{
	if(node_tg3.has_matrix)
	{
		return utils::get_rotation_from_model_matrix(gltf::get_mat4_from_1d_matrix(node_tg3.matrix));
	}
	else
	{
		return glm::quat(float(node_tg3.rotation[3]), float(node_tg3.rotation[0]), float(node_tg3.rotation[1]), float(node_tg3.rotation[2])); //ordre glTF = (x, y, z, w), ordre glm::quat = (w, x, y, z)
	}
}

glm::vec3 get_node_scale(const tg3_node& node_tg3)
{
	if(node_tg3.has_matrix)
	{
		return utils::get_scale_from_model_matrix(gltf::get_mat4_from_1d_matrix(node_tg3.matrix));
	}
	else
	{
		return glm::vec3(node_tg3.scale[0], node_tg3.scale[1], node_tg3.scale[2]);
	}
}

gltf::Node get_node(std::string_view path, const tg3_model& model_tg3, const tg3_node& node_tg3, glm::mat4 parent_matrix)
{
	std::string node_name;
	if(node_tg3.name.len > 0)
	{
		node_name = std::string(node_tg3.name.data);
	}

	gltf::Node node(node_name, get_transform(node_tg3), parent_matrix, get_mesh(path, model_tg3, node_tg3), get_aabb(model_tg3, node_tg3));
	if(node_tg3.children_count > 1 && node_tg3.mesh == -1)
	{
		node.set_empty_node();
	}
	for(uint32_t i = 0; i < node_tg3.children_count; ++i)
	{
		node.add_child(get_node(path, model_tg3, model_tg3.nodes[node_tg3.children[i]], node.compute_model()));
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
		std::string attribute_name_str;
		if(attribute_tg3.key.len > 0)
		{
			attribute_name_str = std::string(attribute_tg3.key.data);
		}

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
			vertices.add_texcoord_attributes(get_float_vec_attribute<2>(model_tg3, attribute_tg3));
		}
		else if(attribute_name_str.find("COLOR_") != std::string::npos) //vec3 ou vec4
		{
			tg3_accessor accessor_tg3 = model_tg3.accessors[attribute_tg3.value];
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
			logging::log("In get_vertices(), the requested attribute does not exist!", logging::Severity::WARNING);
		}
	}
	return vertices;
}

Vertices get_aabb_vertices(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3)
{
	glm::vec3 min_values = get_min_values(model_tg3, primitive_tg3);
	glm::vec3 max_values = get_max_values(model_tg3, primitive_tg3);
	std::size_t vertices_number = 8ULL;

	std::vector<glm::vec3> position_attributes;
	position_attributes.reserve(vertices_number);
	position_attributes.push_back(glm::vec3(max_values.x, min_values.y, min_values.z)); //0 : bottom face, upper right
	position_attributes.push_back(glm::vec3(min_values.x, min_values.y, min_values.z)); //1 : bottom face, upper left
	position_attributes.push_back(glm::vec3(max_values.x, min_values.y, max_values.z)); //2 : bottom face, down right
	position_attributes.push_back(glm::vec3(min_values.x, min_values.y, max_values.z)); //3 : bottom face, down left
	position_attributes.push_back(glm::vec3(max_values.x, max_values.y, min_values.z)); //4 : top face, upper right
	position_attributes.push_back(glm::vec3(min_values.x, max_values.y, min_values.z)); //5 : top face, upper left
	position_attributes.push_back(glm::vec3(max_values.x, max_values.y, max_values.z)); //6 : top face, down right
	position_attributes.push_back(glm::vec3(min_values.x, max_values.y, max_values.z)); //7 : top face, down left

	Vertices vertices(vertices_number);
	vertices.add_position_attributes(position_attributes);
	return vertices;
}

Transform get_transform(const tg3_node& node_tg3)
{
	Transform transform;
	transform.position_ = get_node_position(node_tg3);
	transform.rotation_ = get_node_rotation(node_tg3);
	transform.scale_ = get_node_scale(node_tg3);
	return transform;
}

resource::MeshKey get_mesh(std::string_view path, const tg3_model& model_tg3, const tg3_node& node_tg3)
{
	int32_t mesh_index = node_tg3.mesh;
	resource::MeshKey mesh_key;
	if(mesh_index != -1)
	{
		tg3_mesh mesh_tg3 = model_tg3.meshes[mesh_index];
		if(mesh_tg3.primitives_count > 1)
		{
			logging::log("Number of primitives > 1 not handled", logging::Severity::WARNING);
		}

		tg3_primitive primitive_tg3 = mesh_tg3.primitives[0];
		std::vector<GLushort> ebo_values = get_ebo_values(model_tg3, primitive_tg3);
		Vertices vertices = get_vertices(model_tg3, primitive_tg3);
		if(has_textures(model_tg3))
		{
			std::vector<std::string> texture_keys = get_textures(model_tg3);
			mesh_key = resource::add_mesh(path, mesh_index, ebo_values, vertices, texture_keys, primitive_tg3.mode);
		}
		else
		{
			mesh_key = resource::add_mesh(path, mesh_index, ebo_values, vertices, primitive_tg3.mode);
		}
	}
	return mesh_key;
}

std::optional<tg3_accessor> get_accessor_from_attribute(std::string_view attribute, const tg3_model& model_tg3, const tg3_primitive& primitive_tg3)
{
	if(attribute != "POSITION" && attribute != "NORMAL"
	&& attribute.find("TEXCOORD_") != std::string_view::npos
	&& attribute.find("COLOR_") != std::string_view::npos)
	{
		logging::log("get_accessor_from_attribute() returned std::nullopt!", logging::Severity::CRITICAL);
		return std::nullopt;
	}

	for(uint32_t i = 0; i < primitive_tg3.attributes_count; i++)
	{
		tg3_str_int_pair attribute_tg3 = primitive_tg3.attributes[i];
		std::string attribute_name_str;
		if(attribute_tg3.key.len > 0)
		{
			attribute_name_str = std::string(attribute_tg3.key.data);
		}

		if(attribute_name_str == attribute)
		{
			return model_tg3.accessors[attribute_tg3.value];
		}
	}
	logging::log("get_accessor_from_attribute() returned std::nullopt!", logging::Severity::WARNING);
	return std::nullopt;
}

glm::vec3 get_min_values(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3)
{
	tg3_accessor accessor_tg3 = get_accessor_from_attribute("POSITION", model_tg3, primitive_tg3).value(); //je suis assuré que la méthode retourne quelque chose car "POSITION" est un attribut qui existe donc osef de tester l'optional
	glm::vec3 min_values(0.0f);
	if(accessor_tg3.min_values_count > 0)
	{
		const double* min_values_ptr = accessor_tg3.min_values;
		min_values = glm::vec3(min_values_ptr[0], min_values_ptr[1], min_values_ptr[2]);
	}
	return min_values;
}

glm::vec3 get_max_values(const tg3_model& model_tg3, const tg3_primitive& primitive_tg3)
{
	tg3_accessor accessor_tg3 = get_accessor_from_attribute("POSITION", model_tg3, primitive_tg3).value(); //je suis assuré que la méthode retourne quelque chose car "POSITION" est un attribut qui existe donc osef de tester l'optional
	glm::vec3 max_values(0.0f);
	if(accessor_tg3.max_values_count > 0)
	{
		const double* max_values_ptr = accessor_tg3.max_values;
		max_values = glm::vec3(max_values_ptr[0], max_values_ptr[1], max_values_ptr[2]);
	}
	return max_values;
}

std::optional<AABB> get_aabb(const tg3_model& model_tg3, const tg3_node& node_tg3)
{
	int32_t mesh_index = node_tg3.mesh;
	if(mesh_index != -1)
	{
		tg3_mesh mesh_tg3 = model_tg3.meshes[mesh_index];
		if(mesh_tg3.primitives_count > 1)
		{
			logging::log("Number of primitives > 1 not handled", logging::Severity::WARNING);
		}

		tg3_primitive primitive_tg3 = mesh_tg3.primitives[0];
		Vertices vertices = get_aabb_vertices(model_tg3, primitive_tg3);
		std::vector<GLushort> ebo_values = //ex du rubix cube avec la face verte au-dessus et la face rouge face à nous
		{
			3, 2, 7, 2, 6, 7, //face rouge
			1, 5, 7, 1, 3, 7, //face jaune
			0, 4, 6, 0, 2, 6, //face blanche
			1, 0, 5, 0, 4, 5, //face orange
			7, 5, 4, 4, 6, 7, //face verte
			3, 1, 0, 0, 2, 3  //face bleue
		};
		glm::vec3 min_values = get_min_values(model_tg3, primitive_tg3);
		glm::vec3 max_values = get_max_values(model_tg3, primitive_tg3);

		//un AABB n'a pas de texture
		return AABB(min_values, max_values, ebo_values, vertices, primitive_tg3.mode);
	}
	std::string node_name = (node_tg3.name.len > 0) ? std::string(node_tg3.name.data) : "";
	logging::log("get_aabb() returned std::nullopt (the node \"" + node_name + "\" does not have a AABB)", logging::Severity::NOTICE);
	return std::nullopt; //cas où le node ne possède pas de mesh
}

std::vector<std::string> get_textures(const tg3_model& model_tg3)
{
	std::vector<std::string> texture_keys;
	for(uint32_t i = 0; i < model_tg3.textures_count; ++i)
	{
		tg3_texture texture_tg3 = model_tg3.textures[i];
		tg3_image image_tg3 = model_tg3.images[texture_tg3.source];

		Texture mesh_texture;
		mesh_texture.texture_unit_ = i;

		if(image_tg3.buffer_view == -1)
		{
			std::string image_str;
			if(image_tg3.uri.len > 0)
			{
				image_str = std::string(image_tg3.uri.data);
			}

			if(tg3_is_data_uri(image_tg3.uri.data, image_tg3.uri.len))
			{
				mesh_texture.image_path_ = "";
				std::string image_data_base64 = image_str.substr(image_str.find(',') + 1); //+1 pour ne pas prendre la virgule
				std::string image_data_decoded = utils::base64_decode(image_data_base64.data(), image_data_base64.length());
				mesh_texture.image_data_.reserve(image_data_base64.length());
				for(std::size_t j = 0; j < image_data_decoded.length(); ++j)
				{
					mesh_texture.image_data_.push_back(image_data_decoded[j]);
				}
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

		texture_keys.push_back(resource::add_texture(mesh_texture));
	}
	return texture_keys;
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
	std::vector<glm::vec4> vec4_colors;

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
				glm::vec4 vec4_color(0.0f);
				for(uint64_t j = 0; j < component_type_size * glm::vec4::length(); j += component_type_size)
				{
					uint8_t attribute_u8 = 0; //uint8_t car un unsigned short fait 8 bits
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_u8 |= uint8_t(buffer_tg3.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
					}
					GLfloat attribute_float = GLfloat(attribute_u8) / std::numeric_limits<GLubyte>::max(); //normalisation de la valeur de l'attribut
					uint64_t component = j / component_type_size;
					vec4_color[component % 4] = attribute_float;
				}
				vec4_colors.push_back(vec4_color);
			}
		}
		else if(component_type_str == "GL_UNSIGNED_SHORT")
		{
			for(uint64_t i = buffer_view_tg3.byte_offset + accessor_tg3.byte_offset; i < buffer_view_tg3.byte_offset + buffer_view_tg3.byte_length; i += 4 * sizeof(GLushort)) //car sizeof(glm::vec4) != 4 * sizeof(sizeof(GLushort))
			{
				glm::vec4 vec4_color(0.0f);
				for(uint64_t j = 0; j < component_type_size * glm::vec4::length(); j += component_type_size)
				{
					uint16_t attribute_u16 = 0; //uint16_t car un unsigned short fait 16 bits
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_u16 |= uint16_t(buffer_tg3.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
					}
					GLfloat attribute_float = GLfloat(attribute_u16) / std::numeric_limits<GLushort>::max(); //normalisation de la valeur de l'attribut
					uint64_t component = j / component_type_size;
					vec4_color[component % 4] = attribute_float;
				}
				vec4_colors.push_back(vec4_color);
			}
		}
		else
		{
			logging::log("In get_vec4_color_attribute(), the requested type does not exist!", logging::Severity::WARNING);
		}
	}
	return vec4_colors;
}

bool has_textures(const tg3_model& model_tg3)
{
	return model_tg3.textures_count > 0;
}

}