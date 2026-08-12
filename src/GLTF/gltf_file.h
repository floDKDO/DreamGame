#pragma once

#include "gltf.h"
#include "utils.h"
#include "Render/mesh.h"

#include <tinygltf/tiny_gltf_v3.h>

class glTFFile
{
	public:
		explicit glTFFile(std::string_view path);
		void open();
		void print_info() const;
		tg3_model get_model() const;
		uint64_t get_attributes_count(const tg3_primitive& primitive) const;
		Vertices get_vertices(const tg3_primitive& primitive) const;
		std::vector<Mesh> get_meshes(const tg3_node& node) const;
		std::vector<Texture> get_textures() const;
		std::vector<GLushort> get_ebo_values(const tg3_primitive& primitive) const;
		std::vector<glm::vec4> get_vec4_color_attribute(const tg3_str_int_pair& attribute) const;

		template <glm::length_t L>
		std::vector<glm::vec<L, float>> get_float_vec_attribute(const tg3_str_int_pair& attribute) const
		{
			using vector_type = std::vector<glm::vec<L, float>>;

			tg3_accessor accessor = model_.accessors[attribute.value];
			tg3_buffer_view buffer_view = model_.buffer_views[accessor.buffer_view];
			tg3_buffer buffer = model_.buffers[buffer_view.buffer];

			std::string component_type_str = gltf::get_component_type_str(accessor.component_type);
			std::size_t component_type_size = gltf::get_component_type_size(accessor.component_type);
			std::string type_str = gltf::get_type_str(accessor.type);

			uint64_t stride = buffer_view.byte_stride != 0 ? buffer_view.byte_stride : sizeof(vector_type::value_type);
			vector_type vector;

			for(uint64_t i = buffer_view.byte_offset + accessor.byte_offset; i < buffer_view.byte_offset + buffer_view.byte_length; i += stride)
			{
				vector_type::value_type vector_element(0.0f);
				for(uint64_t j = 0; j < component_type_size * vector_type::value_type::length(); j += component_type_size)
				{
					uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 bits (tous les attributs contiennent des composants de type GL_FLOAT)
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_ieee754 |= uint32_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir les octets en bits
					}
					GLfloat attribute_float = utils::ieee754_to_float(attribute_ieee754);
					uint64_t component = j / component_type_size;
					vector_element[component % L] = attribute_float;
				}
				vector.push_back(vector_element);
			}
			return vector;
		}

	private:
		std::string path_;
		tg3_model model_;
		tg3_error_stack errors_;
};