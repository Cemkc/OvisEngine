#pragma once

#include "Ovis/Renderer/Camera.h"
#include "Ovis/Renderer/Texture.h"
#include "Ovis/GameEntity/GameEntity.h"

namespace Ovis
{
	class Renderer2D
	{
	private:
		static Renderer2D* s_Instance;
	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		Renderer2D();

		inline static Renderer2D& Instance(){ return *s_Instance; }

		virtual ~Renderer2D() = default;

		virtual void Init() = 0;
		virtual void ShutDown() = 0;

		virtual void BeginScene(const OrthographicCamera& camera) = 0;
		virtual void EndScene() = 0;

		virtual void SubmitQuad(const GameEntity& entity, const glm::vec4& color) = 0;
		virtual void SubmitQuad(const GameEntity& entity, const Texture2D& texture, float tilingFactor = 1.0f) = 0;

		virtual void ResetStats() = 0;
		virtual Statistics GetStats() = 0;
	};
}

