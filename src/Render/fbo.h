#pragma once

#include <GL/glew.h>
#include <cstddef>

class FBO
{
	public:
		FBO(int width, int height);

		void render() const;
		void bind() const;

	private:
		int width_, height_;
		std::size_t mesh_id_, texture_id_;
		GLuint rbo_id_, fbo_id_;
};