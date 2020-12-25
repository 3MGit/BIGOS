#pragma once

#include "Engine/Core/Logger.h"


#define BGS_CORE_ASSERT(expr) if (!(expr)) BGS_CORE_FATAL("Assertion failed! \n\t\t\t\t\tFile: %s \n\t\t\t\t\tLine: %d",__FILE__, __LINE__)