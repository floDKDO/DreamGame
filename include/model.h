#include "mesh.h"

//TODO : inclure les third party avec "..." ou <...> ??

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
		std::vector<GLushort> get_ebo_values(const tg3_primitive& primitive) const;
		std::vector<glm::vec2> get_vec2_attribute(const tg3_str_int_pair& attribute) const;
		std::vector<glm::vec3> get_vec3_attribute(const tg3_str_int_pair& attribute) const;

		std::string path_;
		tg3_model model_;
		tg3_error_stack errors_;
		std::vector<Mesh> meshes_;
};