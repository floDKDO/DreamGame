#include "utils.h"

#include <string>
#include <iostream>

namespace utils
{

void GLAPIENTRY message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, [[maybe_unused]] const void* user_param)
{
	auto lambda_source = [&source]() -> std::string
	{
		switch(source)
		{
			case GL_DEBUG_SOURCE_API:
				return "SOURCE: API, ";

			case GL_DEBUG_SOURCE_APPLICATION:
				return "SOURCE: Application, ";

			case GL_DEBUG_SOURCE_OTHER:
				return "SOURCE: Other, ";

			case GL_DEBUG_SOURCE_SHADER_COMPILER:
				return "SOURCE: Shader compiler, ";

			case GL_DEBUG_SOURCE_THIRD_PARTY:
				return "SOURCE: Third party, ";

			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				return "SOURCE: Window system, ";

			default:
				return "SOURCE: Unknown, ";
		}
	};

	auto lamda_type = [&type]() -> std::string
	{
		switch(type)
		{
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				return "Deprecated behavior";

			case GL_DEBUG_TYPE_ERROR:
				return "Error";

			case GL_DEBUG_TYPE_MARKER:
				return "Marker";

			case GL_DEBUG_TYPE_OTHER:
				return "Other";

			case GL_DEBUG_TYPE_PERFORMANCE:
				return "Performance";

			case GL_DEBUG_TYPE_POP_GROUP:
				return "Pop group";

			case GL_DEBUG_TYPE_PORTABILITY:
				return "Portability";

			case GL_DEBUG_TYPE_PUSH_GROUP:
				return "Push group";

			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				return "Undefined behavior";

			default:
				return "Unknown";
		}
	};

	auto lambda_severity = [&severity]() -> std::string
	{
		switch(severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:
				return "SEVERITY: High, ";

			case GL_DEBUG_SEVERITY_LOW:
				return "SEVERITY: Low, ";

			case GL_DEBUG_SEVERITY_MEDIUM:
				return "SEVERITY: Medium, ";

			case GL_DEBUG_SEVERITY_NOTIFICATION:
				return "SEVERITY: Notification, ";

			default:
				return "SEVERITY: Unknown, ";
		}
	};

	std::cout << "[OpenGL " << lamda_type() << "] - " << lambda_severity() << lambda_source() << "ID: " << id << ", MESSAGE (length=" << length << "): <" << message << ">\n";
}

}