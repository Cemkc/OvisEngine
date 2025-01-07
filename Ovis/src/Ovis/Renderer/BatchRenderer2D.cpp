#include "ovpch.h"
#include "BatchRenderer2D.h"

// #include "glad/glad.h"

namespace Ovis
{
	void BatchRenderer2D::Init()
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

		m_QuadVertexBuffer = VertexBuffer::Create(m_MaxVertices * sizeof(Vertex));

		BufferLayout squareLayout
		{
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" }
		};

		m_QuadVertexBuffer->SetLayout(squareLayout);
		m_QuadVertexBufferBase = new Vertex[m_MaxVertices];
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;

		uint32_t* quadIndices = new uint32_t[m_MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < m_MaxIndices; i += 6)
		{
			quadIndices[i + 0] = 0 + offset;
			quadIndices[i + 1] = 1 + offset;
			quadIndices[i + 2] = 2 + offset;

			quadIndices[i + 3] = 2 + offset;
			quadIndices[i + 4] = 3 + offset;
			quadIndices[i + 5] = 0 + offset;

			offset += 4;
		}

		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(quadIndices, m_MaxIndices);

		m_QuadVertexArray = VertexArray::Create();
		m_QuadVertexArray->AddVertexBuffer(m_QuadVertexBuffer);
		m_QuadVertexArray->SetIndexBuffer(squareIndexBuffer);
		delete[] quadIndices;

		m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		m_WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_StandartShader = Shader::Create("assets/shaders/Texture.glsl");
		m_StandartShader->Bind();
		m_StandartShader->SetUniform("u_Texture", 0);
	}

	void BatchRenderer2D::ShutDown()
	{
	}

	void BatchRenderer2D::BeginScene(const OrthographicCamera& camera)
	{
		OV_RENDER_PROFILE_FUNC();

		m_StandartShader->Bind();
		m_StandartShader->SetUniform("u_View", camera.GetViewMatrix());
		m_StandartShader->SetUniform("u_Projection", camera.GetProjectionMatrix());

		m_QuadIndexCount = 0;
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
	}

	void BatchRenderer2D::EndScene()
	{
		OV_RENDER_PROFILE_FUNC();

		m_QuadVertexArray->Bind();

		uint32_t bufferSize = (m_QuadVertexBufferPtr - m_QuadVertexBufferBase) * sizeof(uint32_t);
		m_QuadVertexBuffer->SetData(m_QuadVertexBufferBase, bufferSize);

		Flush();
	}

	void BatchRenderer2D::Flush()
	{
		RenderCommand::DrawIndexed(m_QuadVertexArray, m_QuadIndexCount);
	}

	void BatchRenderer2D::SubmitQuad(const Transform& transform, const glm::vec4& color)
	{
		OV_RENDER_PROFILE_FUNC();

		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 0.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 0.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 1.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 1.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

		m_QuadIndexCount += 6;

	}

	void BatchRenderer2D::SubmitQuad(const Transform& transform, const Texture2D& texture, float tilingFactor)
	{
		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = glm::vec4(1.0f);
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 0.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = glm::vec4(1.0f);
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 0.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = glm::vec4(1.0f);
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 1.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = transform.Position;
		m_QuadVertexBufferPtr->Color = glm::vec4(1.0f);
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 1.0f };
		m_QuadVertexBufferPtr->TexId = 0;
		m_QuadVertexBufferPtr++;

	}

}

