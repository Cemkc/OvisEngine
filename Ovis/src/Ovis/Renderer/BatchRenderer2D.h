#pragma once

#include "Renderer2D.h"

#include "Vertex.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Ovis
{
	class BatchRenderer2D : public Renderer2D
	{
	private:
		float m_QuadSize = 1.0f;

		static const uint32_t s_MaxQuads = 10000;
		static const uint32_t s_MaxVertices = s_MaxQuads * 4;
		static const uint32_t s_MaxIndices = s_MaxQuads * 6;

		uint32_t m_QuadIndexCount = 0;
		static const uint32_t s_MaxTextureSlots = 32;
		static const Texture2D* s_TextureSlots[s_MaxTextureSlots];
		uint32_t m_TextureSlotIndex;

		Vertex* m_QuadVertexBufferBase = nullptr;
		Vertex* m_QuadVertexBufferPtr = nullptr;

		std::shared_ptr<VertexArray> m_QuadVertexArray;
		std::shared_ptr<VertexBuffer> m_QuadVertexBuffer;

		std::shared_ptr<Shader> m_StandartShader;
		std::shared_ptr<Texture2D> m_WhiteTexture;

		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	public:
		void Init() override;
		void ShutDown() override;
		void BeginScene(const OrthographicCamera& camera) override;
		void EndScene() override;
		void SubmitQuad(const Transform& transform, const glm::vec4& color) override;
		void SubmitQuad(const Transform& transform, const Texture2D& texture, float tilingFactor) override;
	private:
		void Flush();
	};
}