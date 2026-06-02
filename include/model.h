#include "mesh.h"
#include "tinygltf/tiny_gltf_v3.h"
#include "gl/glew.h"

#include <glm/gtc/type_ptr.hpp>
#include <string_view>
#include <string>
#include <vector>

class Model
{
	public:
		explicit Model(std::string_view path);
		Model(const Model& model) = delete;
		Model(Model&& model) = delete;
		Model& operator=(const Model& model) = delete;
		Model& operator=(Model&& model) = delete;
		~Model();

	private:
		void open_gltf_file(std::string_view path);
		void load_meshes();

		std::string get_target_str(int32_t target) const;
		std::string get_type_str(int32_t type) const;
		std::string get_component_type_str(int32_t component_type) const;
		GLfloat ieee754_to_float(uint64_t ieee754_number) const;
		std::vector<GLushort> get_ebo_values(const tg3_primitive& primitive) const;
		std::vector<glm::vec3> get_position_attribute_values(const tg3_str_int_pair& attribute) const;

		tg3_model model_;
		tg3_error_stack errors_;
		std::vector<Mesh> meshes_;
};