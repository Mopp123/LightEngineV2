
#include "Debug.h"
#include <stdio.h>

namespace lightEngine
{

	size_t Debug::dynamic_memory_allocated = 0;

	void Debug::log(const std::string& m)
	{
		printf("%s\n", m.c_str());
	}

	void Debug::trigger_fatal_error()
	{

	}
}

/*
void* operator new(size_t size)
{
	lightEngine::Debug::dynamic_memory_allocated += size;

	return malloc(size);
}

void operator delete(void* mem, size_t size)
{
	lightEngine::Debug::dynamic_memory_allocated -= size;

	free(mem);
}

void operator delete[](void* mem, size_t size)
{
	lightEngine::Debug::dynamic_memory_allocated -= size;

	free(mem);
}*/