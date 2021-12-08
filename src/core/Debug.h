#pragma once

#include <string>

#define LIGHT_ENGINE__DEBUG_MODE__FULL
//#define LIGHT_ENGINE__DEBUG_MODE__SMALL

// Macros..
//	..quite disgusting..
#define glCall(func)	func;\
switch (glGetError())\
{\
case GL_NO_ERROR:\
	break;\
case GL_INVALID_ENUM:					Debug::log("GL ERROR: GL_INVALID_ENUM");Debug::trigger_fatal_error();\
	break;\
case GL_INVALID_VALUE:					Debug::log("GL ERROR: GL_INVALID_VALUE");Debug::trigger_fatal_error();\
	break;\
case GL_INVALID_OPERATION:				Debug::log("GL ERROR: GL_INVALID_OPERATION");Debug::trigger_fatal_error();\
	break;\
case GL_INVALID_FRAMEBUFFER_OPERATION:	Debug::log("GL ERROR: GL_INVALID_FRAMEBUFFER_OPERATION");Debug::trigger_fatal_error();\
	break;\
case GL_OUT_OF_MEMORY:					Debug::log("GL ERROR: GL_OUT_OF_MEMORY");Debug::trigger_fatal_error();\
	break;\
case GL_STACK_UNDERFLOW:				Debug::log("GL ERROR: GL_STACK_UNDERFLOW");Debug::trigger_fatal_error();\
	break;\
case GL_STACK_OVERFLOW:					Debug::log("GL ERROR: GL_STACK_OVERFLOW");Debug::trigger_fatal_error();\
	break;\
default:\
	break;\
}

namespace lightEngine
{
	class Debug
	{
	public:

		static size_t dynamic_memory_allocated;

	public:

		static void log(const std::string& m);

		static void trigger_fatal_error();

	};
}

/*
// Ths was quite fucked, especially because of new[] and delete[]
void* operator new(size_t size);

void operator delete(void* mem, size_t size);
void operator delete[](void* mem, size_t size); // <- this one kind of doesnt work..
*/