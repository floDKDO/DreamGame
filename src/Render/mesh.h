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
		};

		Mesh(const MeshId& mesh_id, const MeshInfo& mesh_info);

		void draw() const;
		void destroy();

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

inline bool operator<(const Mesh::MeshId& a, const Mesh::MeshId& b)
{
	if(a.file_name_ < b.file_name_)
	{
		return true;
	}
	else if(b.file_name_ < a.file_name_)
	{
		return false;
	}
	else if(a.mesh_index_ < b.mesh_index_)
	{
		return true;
	}
	else
	{
		return false;
	}

	//return (a.file_name_ < b.file_name_) || (a.mesh_index_ < b.mesh_index_);
	//TODO : ne marche pas pour ce cas-là : 
	//a = "aaa", 42 ; b = "zzz", 1
	//a < b => true
	//b < a => true
}