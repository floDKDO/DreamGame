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
		std::vector<glm::vec2> get_vec2_attribute(const tg3_str_int_pair& attribute) const;
		std::vector<glm::vec3> get_vec3_attribute(const tg3_str_int_pair& attribute) const;
		Vertices get_vertices(const tg3_primitive& primitive);

		std::string path_;
		tg3_model model_;
		tg3_error_stack errors_;
		std::vector<Node> nodes_;
		std::vector<Mesh> meshes_;
};