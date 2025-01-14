#include "ovpch.h"
#include "SnapRenderer2D.h"

#include "Ovis/Core/Application.h"

namespace Ovis
{
	void SnapRenderer2D::Init()
	{
		OV_RENDER_PROFILE_FUNC();

		float squareVertices[] =
		{
			-1.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-1.5f,  0.5f, 0.0f, 0.0f, 1.0f,

			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 1.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 1.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<VertexBuffer> squareVertexBuffer;
		squareVertexBuffer = VertexBuffer::Create(squareVertices, sizeof(squareVertices));

		BufferLayout squareLayout
		{
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};

		squareVertexBuffer->SetLayout(squareLayout);

		uint32_t squareIndices[] =
		{
			0, 1, 2, 2, 3, 0,
			4, 5, 6, 6, 7, 4
		};

		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		m_Storage.QuadVertexArray = VertexArray::Create();
		m_Storage.QuadVertexArray->AddVertexBuffer(squareVertexBuffer);
		m_Storage.QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		m_Storage.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		m_Storage.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_Storage.StandartShader = Shader::Create("assets/shaders/SnapShader.glsl");
		m_Storage.StandartShader->Bind();
		m_Storage.StandartShader->SetUniform("u_Texture", 0);
	}

	void SnapRenderer2D::ShutDown()
	{
		OV_RENDER_PROFILE_FUNC();
	}

	void SnapRenderer2D::BeginScene(const OrthographicCamera& camera)
	{
		m_Storage.ViewMatrix = camera.GetViewMatrix();
		m_Storage.ProjectionMatrix = camera.GetProjectionMatrix();
	}

	void SnapRenderer2D::EndScene()
	{
	}

	void SnapRenderer2D::SubmitQuad(const Transform& transform, const glm::vec4& color)
	{
		OV_RENDER_PROFILE_FUNC();

		m_Storage.StandartShader->SetUniform("u_Color", color);
		m_Storage.WhiteTexture->Bind();

		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		if (transform.Rotation.x != 0 || transform.Rotation.y != 0 || transform.Rotation.z != 0) //  This saves us from calculating a rotation matrix which can be costly if no rotation is applied
			model = glm::rotate(model, glm::radians(transform.Rotation.z), { 0.0f, 0.0f, 1.0f });

		m_Storage.StandartShader->SetUniform("u_model", model);
		m_Storage.StandartShader->SetUniform("u_view", m_Storage.ViewMatrix);
		m_Storage.StandartShader->SetUniform("u_projection", m_Storage.ProjectionMatrix);

		m_Storage.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Storage.QuadVertexArray);

		m_Storage.Stats.DrawCalls++;
		m_Storage.Stats.QuadCount++;
	}

	void SnapRenderer2D::SubmitQuad(const Transform& transform, const Texture2D& texture, float tilingFactor)
	{
		OV_RENDER_PROFILE_FUNC();

		m_Storage.StandartShader->Bind();
		m_Storage.StandartShader->SetUniform("u_Color", glm::vec4(1.0f));
		m_Storage.StandartShader->SetUniform("u_TilingFactor", tilingFactor);
		texture.Bind();

		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		if (transform.Rotation.x != 0 || transform.Rotation.y != 0 || transform.Rotation.z != 0) //  This saves us from calculating a rotation matrix which can be costly if no rotation is applied
			model = glm::rotate(model, glm::radians(transform.Rotation.z), { 0.0f, 0.0f, 1.0f });

		m_Storage.StandartShader->SetUniform("u_model", model);
		m_Storage.StandartShader->SetUniform("u_view", m_Storage.ViewMatrix);
		m_Storage.StandartShader->SetUniform("u_projection", m_Storage.ProjectionMatrix);

		m_Storage.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Storage.QuadVertexArray);

		m_Storage.Stats.DrawCalls++;
		m_Storage.Stats.QuadCount++;
	}

	void SnapRenderer2D::ResetStats()
	{
		memset(&m_Storage.Stats, 0, sizeof(Statistics));
	}

	Renderer2D::Statistics SnapRenderer2D::GetStats()
	{
		return m_Storage.Stats;
	}

}
