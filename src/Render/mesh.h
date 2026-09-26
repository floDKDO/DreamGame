#pragma once

#include "vertices.h"

class Mesh
{
	public: 
		struct MeshInfo
		{
			std::vector<GLushort> ebo_values_;
			Vertices vertices_;
			std::vector<std::string> texture_keys_ = {}; //garder l'init à vide pour l'instant (à retirer quand tout les meshes utiliseront des textures)
			GLenum draw_mode_ = GL_TRIANGLES;
		};

		struct MeshId
		{
			std::string file_name_ = "";
			int32_t mesh_index_ = -1;

			bool operator==(const MeshId& other) const
			{
				return (file_name_ == other.file_name_ && mesh_index_ == other.mesh_index_);
			}
		};

		Mesh(const MeshId& mesh_id, const MeshInfo& mesh_info);
		Mesh(const Mesh&) = delete;
		Mesh(Mesh&& mesh);
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&& mesh);
		~Mesh();

		void draw() const;

	private:
		void load_vertex_attribute(GLuint vbo_binding_index, attribute::Name attribute_name);
		void create_ebo();
		void create_vbo();
		void create_vao();
		void destroy_all_buffers() const;
		void create_textures();
		void load_mesh();

		MeshId mesh_id_;
		MeshInfo mesh_info_;
		GLuint ebo_, vbo_, vao_;
};

template <>
struct std::hash<Mesh::MeshId>
{
	std::size_t operator()(const Mesh::MeshId& mesh_id) const
	{
		return (std::hash<std::string>()(mesh_id.file_name_) ^ (std::hash<int32_t>()(mesh_id.mesh_index_) << 1));
	}
};