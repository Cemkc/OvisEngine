#pragma once

#include "Renderer2D.h"

#include "Vertex.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Ovis/GameEntity/GameEntity.h"

namespace Ovis
{
	class BatchRenderer2D : public Renderer2D
	{
	public:
		void Init() override;
		void ShutDown() override;
		void BeginScene(const OrthographicCamera& camera) override;
		void EndScene() override;
		void SubmitQuad(const GameEntity& entity, const glm::vec4& color) override;
		void SubmitQuad(const GameEntity& entity, const Texture2D& texture, float tilingFactor) override;

		void ResetStats() override ;
		Statistics GetStats() override { return s_Stats; }

	private:
		float m_QuadSize = 1.0f;

		static const uint32_t s_MaxQuads = 20000;
		static const uint32_t s_MaxVertices = s_MaxQuads * 4;
		static const uint32_t s_MaxIndices = s_MaxQuads * 6;
		static const glm::vec4 s_QuadCorners[4];

		static Statistics s_Stats;

		uint32_t m_QuadIndexCount = 0;
		static const uint32_t s_MaxTextureSlots = 32;
		static const Texture2D* s_TextureSlots[s_MaxTextureSlots];
		uint32_t m_TextureSlotIndex;

		QuadVertex* m_QuadVertexBufferBase = nullptr;
		QuadVertex* m_QuadVertexBufferPtr = nullptr;

		std::shared_ptr<VertexArray> m_QuadVertexArray;
		std::shared_ptr<VertexBuffer> m_QuadVertexBuffer;

		std::shared_ptr<Shader> m_StandartShader;
		std::shared_ptr<Texture2D> m_WhiteTexture;

		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;

		void Flush();
		void FlushAndReset();
	};
}