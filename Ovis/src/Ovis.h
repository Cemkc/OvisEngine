#pragma once

// For use by Ovis Applications

// Precompiled header
#include "ovpch.h"

#include "Ovis/Core/Application.h"
#include "Ovis/Core/Log.h"
#include "Ovis/Core/Layer.h"

#include "Ovis/Core/Time.h"

#include "Ovis/Core/Input.h"
#include "Ovis/Core/KeyCodes.h"
#include "Ovis/Core/MouseButtonCodes.h"

#include "Ovis/imgui/ImguiLayer.h"

// Renderer
#include "Ovis/Renderer/Renderer.h"
#include "Ovis/Renderer/Renderer2D.h"
#include "Ovis/Renderer/BatchRenderer2D.h"
#include "Ovis/Renderer/SnapRenderer2D.h"
#include "Ovis/Renderer/RenderCommand.h"

#include "Ovis/Renderer/Buffer.h"
#include "Ovis/Renderer/Shader.h"
#include "Ovis/Renderer/Texture.h"
#include "Ovis/Renderer/VertexArray.h"

#include "Ovis/Renderer/Camera.h"
#include "Ovis/Renderer/OrthographicCameraController.h"

// Physics
#include "Ovis/Physics/Ray.h"

// Objects 
#include "Ovis/GameEntity/Transform.h"
#include "Ovis/GameEntity/GameEntity.h"
#include "Ovis/GameEntity/EntityManager.h"

//Utils
#include "Ovis/Utils/MatrixUtils.h"