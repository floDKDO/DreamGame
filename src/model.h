#include "mesh.h"
#include "gltf.h"

#include <tinygltf/tiny_gltf_v3.h>

class Model
{
	public:
		explicit Model(std::string_view path);
		Model(const Model& model) = delete;
		Model(Model&& model) = delete;
		Model& operator=(const Model& model) = delete;
		Model& operator=(Model&& model) = delete;
		~Model();

		void draw(ShaderProgram& shader_program);
		void print_info_gltf() const;

	private:
		struct Node
		{
			Node()
				: mesh_index_(-1), transformation_matrix_(glm::mat4(1.0f))
			{}

			Node(int32_t mesh_index, double rotation[4], double scale[3], double translation[3])
				: mesh_index_(mesh_index), transformation_matrix_(gltf::get_transformation_matrix(rotation, scale, translation))
			{}

			Node(int32_t mesh_index, double matrix[16])
				: mesh_index_(mesh_index), transformation_matrix_(gltf::get_mat4_from_1d_matrix(matrix))
			{}

			int32_t mesh_index_;
			glm::mat4 transformation_matrix_;
		};

		void open_gltf_file();
		void load_meshes();
		uint64_t get_attributes_count(const tg3_primitive& primitive) const;
		std::vector<GLushort> get_ebo_values(const tg3_primitive& primitive) const;
		std::vector<glm::vec4> get_vec4_color_attribute(const tg3_str_int_pair& attribute) const;
		std::vector<glm::vec4> vec3_to_vec4_colors(std::vector<glm::vec3> vector) const;
		Vertices get_vertices(const tg3_primitive& primitive);

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
					uint32_t attribute_ieee754 = 0; //uint32_t car un float fait 32 bits
					for(uint64_t k = 0; k < component_type_size; k += sizeof(uint8_t))
					{
						attribute_ieee754 |= uint32_t(buffer.data.data[i + j + k] << k * 8); //8 pour convertir octets en bits
					}
					GLfloat attribute_float = gltf::ieee754_to_float(attribute_ieee754);
					uint64_t component = j / component_type_size;
					vector_element[component % L] = attribute_float;
				}
				vector.push_back(vector_element);
			}
			return vector;
		}

		std::string path_;
		tg3_model model_;
		tg3_error_stack errors_;
		std::vector<Node> nodes_;
		std::vector<Mesh> meshes_;
};