#include "ovpch.h"

#include "Renderer2D.h"

namespace Ovis
{
	Renderer2D* Renderer2D::s_Instance = nullptr;

	Renderer2D::Renderer2D()
	{
		if (s_Instance != nullptr) OV_CORE_ASSERT(false, "Renderer2D Already Exists!");
		s_Instance = this;
	}
}