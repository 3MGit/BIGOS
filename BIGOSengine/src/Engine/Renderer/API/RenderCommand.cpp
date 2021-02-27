#include "bgspch.h"

#include "Engine/Renderer/API/RenderCommand.h"

namespace BIGOS {

	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}