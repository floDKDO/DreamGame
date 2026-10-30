#pragma once

#include "vertices.h"
#include "texture.h"

class Mesh
{
	public: 
		struct Info
		{
			std::vector<GLushort> ebo_values_;
			Vertices vertices_;
			std::vector<std::size_t> texture_ids_ = {};
			GLenum render_mode_ = GL_TRIANGLES;
		};

		struct Id
		{
			std::string file_name_;
			int32_t mesh_index_ = absent_mesh_index;

			bool operator==(const Id& other) const
			{
				return (file_name_ == other.file_name_ && mesh_index_ == other.mesh_index_);
			}
		};

		Mesh(const Id& mesh_id, const Info& mesh_info);
		Mesh(const Mesh&) = delete;
		Mesh(Mesh&& mesh);
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&& mesh);
		~Mesh();

		void render(GLenum mode = GL_FILL) const;
		void edit_text_texture(int new_width, int new_height, void* new_pixels) const;
		int get_original_texture_width() const;
		int get_original_texture_height() const;

		static const int32_t absent_mesh_index;

	private:
		void load_mesh();
		void load_vertex_attribute(GLuint vbo_binding_index, attribute::Name attribute_name);
		void create_ebo();
		void create_vbo();
		void create_vao();
		void destroy_all_buffers();
		void init_texture(Texture* texture) const;
		void create_and_bind_texture(Texture* texture) const;
		void set_texture_storage(Texture* texture, int width, int height) const;
		void set_texture_parameters(const Texture* texture) const;
		void set_2d_texture_content(Texture* texture, int width, int height, void* pixels) const;
		void set_cubemap_content(Texture* texture) const;
		void create_textures();

		Id mesh_id_;
		Info mesh_info_;
		GLuint ebo_, vbo_, vao_;
};

template <>
struct std::hash<Mesh::Id>
{
	std::size_t operator()(const Mesh::Id& mesh_id) const
	{
		return (std::hash<std::string>()(mesh_id.file_name_) ^ (std::hash<int32_t>()(mesh_id.mesh_index_) << 1));
	}
};