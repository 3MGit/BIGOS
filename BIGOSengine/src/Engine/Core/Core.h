#pragma once

#include "Engine/Utils/Logger.h"
#include "Engine/Utils/ScopedTimer.h"

#define BGS_CORE_ASSERT(x, ...) { if(!(x)) { BGS_CORE_FATAL("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

//#define BGS_CORE_ASSERT(expr, info) if (!(expr)) BGS_CORE_FATAL("Assertion failed! \n\t\t\t\t\tFile: %s \n\t\t\t\t\tLine: %d \n\t\t\t\t\tInfo: %s",__FILE__, __LINE__, info)
//#define BGS_CORE_ASSERT(expr) if (!(expr)) BGS_CORE_FATAL("Assertion failed! \n\t\t\t\t\tFile: %s \n\t\t\t\t\tLine: %d",__FILE__, __LINE__)

#define BIT(x) (1 << x)

#define BGS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

