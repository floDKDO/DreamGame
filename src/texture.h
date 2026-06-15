#include <GL/glew.h>
#include <string>

//TODO : constructeur ? 
struct Texture
{
	//TODO : public/private et mettre dans un fichier à part ??
	//TODO : constructeur ? 
	struct TextureInfo
	{
		GLuint texture_index_;
		std::string image_path_;
		GLint mag_filter_;
		GLint min_filter_;
		GLint wrap_s_;
		GLint wrap_t_;
	};

	GLuint texture_id_;
	TextureInfo texture_info_;
};