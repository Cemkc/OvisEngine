#pragma once

#include "Renderer2D.h"

#include "Vertex.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Ovis
{
	class SnapRenderer2D : public Renderer2D
	{
	private:
		struct Renderer2DStorage
		{
			std::shared_ptr<VertexArray> QuadVertexArray;
			std::shared_ptr<Shader> StandartShader;
			std::shared_ptr<Texture2D> WhiteTexture;
			glm::mat4 ViewMatrix;
			glm::mat4 ProjectionMatrix;

			std::vector<Vertex> VertexData;
		};

		Renderer2DStorage m_Storage;
	public:
		void Init() override;
		void ShutDown() override;
		void BeginScene(const OrthographicCamera& camera) override;
		void EndScene() override;
		void SubmitQuad(const Transform& transform, const glm::vec4& color) override;
		void SubmitQuad(const Transform& transform, const Texture2D& texture, float tilingFactor) override;
	};
}
