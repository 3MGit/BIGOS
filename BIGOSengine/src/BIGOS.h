#pragma once


// For use by BIGOS applications

#include "Engine/Core/App/Application.h"

//-----Utils--------------------------
#include "Engine/Utils/Logger.h"
// -----------------------------------

// ---Entry Point---------------------
//#include "Engine/Core/EntryPoint.h"
// -----------------------------------

//-----Input--------------------------
#include "Engine/Core/App/Input.h"
#include "Engine/Core/KeyCodes.h"
#include "Engine/Core/MouseCodes.h"
// -----------------------------------

//-----Input--------------------------
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/Events/Event.h"
#include "Engine/Events/MouseEvent.h"
#include "Engine/Events/KeyEvent.h"
// -----------------------------------

//-----Renderer-----------------------
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/Camera/EditorCamera.h"
#include "Engine/Renderer/Light.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Mesh.h"
#include "Engine/Renderer/MeshGenerator.h"

#include "Engine/Renderer/API/RenderCommand.h"
#include "Engine/Renderer/API/GraphicsContext.h"
#include "Engine/Renderer/API/Buffer.h"
#include "Engine/Renderer/API/Framebuffer.h"
#include "Engine/Renderer/API/Shader.h"
#include "Engine/Renderer/API/Texture.h"
// -----------------------------------