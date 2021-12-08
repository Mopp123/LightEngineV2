#pragma once

#include "Debug.h"

#define delete_dynamic(obj)						delete obj; obj = 0;
#define delete_dynamic_array(obj)				delete[] obj; obj = 0;

#define allocate_dynamic(Type)					new Type;
#define allocate_dynamic_array(Type, length)	new Type[length];

namespace lightEngine
{
}