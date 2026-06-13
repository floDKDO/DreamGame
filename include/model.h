#include "mesh.h"

//TODO : inclure les third party avec "..." ou <...> ??

#include "tinygltf/tiny_gltf_v3.h"

//TODO : mettre ce namespace dans un fichier à part ?
namespace gltf
{

glm::mat4 get_mat4_from_1d_matrix(double m[16]);
glm::mat4 get_transformation_matrix(double rotation[4], double scale[3], double translation[3]);
void print_mat4(glm::mat4 m);
void print_1d_matrix(double m[16]);
bool is_mat4_identity(glm::mat4 m);
bool is_1d_matrix_identity(double m[16]);
std::string get_target_str(int32_t target);
std::string get_type_str(int32_t type);
std::string get_component_type_str(int32_t component_type);
std::size_t get_component_type_size(int32_t component_type);
std::string get_filter_str(int32_t filter);
std::string get_wrap_str(int32_t wrap);
GLfloat ieee754_to_float(uint64_t ieee754_number);

}


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
		Mesh::Vertices get_vertices(const tg3_primitive& primitive);

		std::string path_;
		tg3_model model_;
		tg3_error_stack errors_;
		std::vector<Node> nodes_;
		std::vector<Mesh> meshes_;
};