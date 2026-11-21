#pragma once

#include <GL/glew.h>

#include "Backend/backend.h"

class FBO
{
	public:
		FBO(Backend& backend, int width, int height);

		void render() const;
		void bind() const;

	private:
		Backend& backend_;
		int width_, height_;
		std::size_t mesh_id_, texture_id_;
		GLuint rbo_id_, fbo_id_;
};