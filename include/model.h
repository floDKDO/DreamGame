#include "mesh.h"
#include "tinygltf/tiny_gltf_v3.h"

#include <glm/vec2.hpp>

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
		void open_gltf_file();
		void load_meshes();

		std::string get_target_str(int32_t target) const;
		std::string get_type_str(int32_t type) const;
		std::string get_component_type_str(int32_t component_type) const;
		std::size_t get_component_type_size(int32_t component_type) const;
		GLfloat ieee754_to_float(uint64_t ieee754_number) const;
		std::vector<GLushort> get_ebo_values(const tg3_primitive& primitive) const;
		std::vector<glm::vec2> get_vec2_attribute(const tg3_str_int_pair& attribute) const;
		std::vector<glm::vec3> get_vec3_attribute(const tg3_str_int_pair& attribute) const;
		//std::vector<Mesh::Vertex> get_vertex_attributes(const tg3_str_int_pair& attribute) const;

		std::string path_;
		tg3_model model_;
		tg3_error_stack errors_;
		std::vector<Mesh> meshes_;
};