#include "mesh.h"

//TODO : inclure les third party avec "..." ou <...> ??

#include "tinygltf/tiny_gltf_v3.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>


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
				: mesh_index_(-1), rotation_(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)), scale_(glm::vec3(1.0f, 1.0f, 1.0f)), translation_(glm::vec3(0.0f, 0.0f, 0.0f))
			{}

			Node(int32_t mesh_index, double rotation[4], double scale[3], double translation[3])
				: mesh_index_(mesh_index), rotation_(glm::vec4(rotation[0], rotation[1], rotation[2], rotation[3])), 
				  scale_(glm::vec3(scale[0], scale[1], scale[2])), translation_(glm::vec3(translation[0], translation[1], translation[2]))
			{}

			glm::mat4 get_transformation_matrix() const
			{
				glm::mat4 matrix = glm::mat4(1.0f);
				matrix = glm::scale(matrix, scale_);
				if(rotation_.x != 0.0f || rotation_.y != 0.0f || rotation_.z != 0.0f)
				{
					matrix = glm::rotate(matrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //glm::acos donne l'angle en radian donc pas besoin de glm::radians
				}
				matrix = glm::translate(matrix, translation_); 
				return matrix;
			}

			int32_t mesh_index_;
			glm::vec4 rotation_;
			glm::vec3 scale_;
			glm::vec3 translation_;
		};

		void open_gltf_file();
		void load_meshes();
		uint64_t get_attributes_count(const tg3_primitive& primitive) const;
		std::vector<GLushort> get_ebo_values(const tg3_primitive& primitive) const;
		std::vector<glm::vec2> get_vec2_attribute(const tg3_str_int_pair& attribute) const;
		std::vector<glm::vec3> get_vec3_attribute(const tg3_str_int_pair& attribute) const;
		Mesh::Vertices get_vertices(const tg3_primitive& primitive);

		std::string path_;
		tg3_model model_;
		tg3_error_stack errors_;
		std::vector<Node> nodes_;
		std::vector<Mesh> meshes_;
};