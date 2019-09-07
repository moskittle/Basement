#pragma once

#include "Basement/Core.h"

#include <unordered_map>

#include <glad/glad.h>

namespace Basement {

	static std::unordered_map<GLenum, std::string> OpenGLErrors
	{
		{ GL_INVALID_ENUM,					"invalid enum" },
		{ GL_INVALID_VALUE,					"invalid value"},
		{ GL_INVALID_OPERATION,				"invalid operation"},
		{ GL_STACK_OVERFLOW,				"stack overflow"},
		{ GL_STACK_UNDERFLOW,				"stack underflow"},
		{ GL_OUT_OF_MEMORY,					"out of memory"},
		{ GL_INVALID_FRAMEBUFFER_OPERATION, "invalid framebuffer operation"},
		{ GL_CONTEXT_LOST,					"context lost"}
	};

	enum class OpenGLErrorCode
	{
		NONE = 0,
		INVALID_ENUM = 0x0500,
		INVALID_VALUE = 0x0501,
		INVALID_OPERATION = 0x0502,
		STACK_OVERFLOW = 0x0503,
		STACK_UNDERFLOW = 0x0504,
		OUT_OF_MEMORY = 0x0505,
		INVALID_FRAMEBUFFER_OPERATION = 0x0506,
		CONTEXT_LOST = 0x0507,
	};
	
#ifdef BM_ENABLE_OPENGL_DEBUG
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
	#define OpenGLCall(x) while (glGetError() != GL_NO_ERROR);\
				x;\
				if(GLenum error = glGetError())\
				{\
					std::stringstream ss; ss << std::hex << error;\
					BM_CORE_ERROR("{0}({1}): error 0x0{2}: {3}", __FILENAME__, __LINE__, ss.str(), OpenGLErrors[error]);\
					/*__debugbreak();*/\
				}
#else
#define OpenGLCallBack(x)
#endif // BM_ENABLE_OPENGL_DEBUG

}
