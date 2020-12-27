#pragma once

#include "Engine/Utils/Logger.h"

#define BGS_CORE_ASSERT(expr) if (!(expr)) BGS_CORE_FATAL("Assertion failed! \n\t\t\t\t\tFile: %s \n\t\t\t\t\tLine: %d",__FILE__, __LINE__)

#define BIT(x) (1 << x)

#define BGS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

