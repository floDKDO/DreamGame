#pragma once

#include "gltf.h"
#include "model.h"

#include <tinygltf/tiny_gltf_v3.h>

namespace gltf
{

class glTFFile
{
	public:
		explicit glTFFile(std::string_view path);

		std::vector<Model> get_models() const;
		glm::vec3 get_node_position(const tg3_node& node_tg3) const;
		glm::vec4 get_node_rotation(const tg3_node& node_tg3) const;
		glm::vec3 get_node_scale(const tg3_node& node_tg3) const;

	private:
		void open();
		void print_info() const;
		Node get_node_and_its_children(const tg3_node& node_tg3, glm::mat4 parent_matrix) const;
		uint64_t get_attributes_count(const tg3_primitive& primitive_tg3) const;
		Vertices get_vertices(const tg3_primitive& primitive_tg3) const;
		Transform get_transform(const tg3_node& node_tg3) const;
		std::optional<Mesh> get_mesh(const tg3_node& node_tg3) const;
		std::vector<Texture> get_textures() const;
		std::vector<GLushort> get_ebo_values(const tg3_primitive& primitive_tg3) const;
		std::vector<glm::vec4> get_vec4_color_attribute(const tg3_str_int_pair& attribute_tg3) const;

		template <glm::length_t L>
		std::vector<glm::vec<L, float>> get_float_vec_attribute(const tg3_str_int_pair& attribute_tg3) const
		{
			using vector_type = std::vector<glm::vec<L, float>>;

			tg3_accessor accessor_tg3 = model_tg3_.accessors[attribute_tg3.value];
			tg3_buffer_view buffer_view_tg3 = model_tg3_.buffer_views[accessor_tg3.buffer_view];
			tg3_buffer buffer_tg3 = model_tg3_.buffers[buffer_view_tg3.buffer];

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

		std::string path_;
		tg3_model model_tg3_;
		tg3_error_stack error_stack_tg3_;
};

}