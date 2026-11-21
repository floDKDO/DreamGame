#include "fbo.h"
#include "Resource/resource_manager.h"
#include "Common/utils.h"
#include "Logging/logging.h"

FBO::FBO(int width, int height)
	: width_(width), height_(height)
{
	texture_id_ = resource::add_texture(Texture{TextureInfo{}, SamplingParameters{}, width_, height_});
	mesh_id_ = resource::add_mesh(Mesh::Info{utils::get_screen_ebo_values(), utils::get_screen_vertices(), {texture_id_}});

	glCreateRenderbuffers(1, &rbo_id_);
	glNamedRenderbufferStorage(rbo_id_, GL_DEPTH24_STENCIL8, width_, height_);

	glCreateFramebuffers(1, &fbo_id_);
	glNamedFramebufferTexture(fbo_id_, GL_COLOR_ATTACHMENT0, resource::get_texture(texture_id_)->info_.id_, 0);
	glNamedFramebufferRenderbuffer(fbo_id_, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo_id_);

	if(glCheckNamedFramebufferStatus(fbo_id_, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		logging::log("Framebuffer not complete!", logging::Severity::WARNING);
	}
}

void FBO::render() const
{
	resource::get_mesh(mesh_id_)->render();
}

void FBO::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id_);
	glViewport(0, 0, width_, height_);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}