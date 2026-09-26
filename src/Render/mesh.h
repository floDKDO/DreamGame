#pragma once

#include "vertices.h"

class Mesh
{
	public: 
		struct Info
		{
			std::vector<GLushort> ebo_values_;
			Vertices vertices_;
			std::vector<std::string> texture_keys_ = {}; //garder l'init à vide pour l'instant (à retirer quand tout les meshes utiliseront des textures)
			GLenum render_mode_ = GL_TRIANGLES;
		};

		struct Id
		{
			std::string file_name_ = "";
			int32_t mesh_index_ = -1;

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

		void render() const;

	private:
		void load_vertex_attribute(GLuint vbo_binding_index, attribute::Name attribute_name);
		void create_ebo();
		void create_vbo();
		void create_vao();
		void destroy_all_buffers() const;
		void create_textures();
		void load_mesh();

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