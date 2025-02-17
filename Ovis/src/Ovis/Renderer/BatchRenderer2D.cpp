#include "ovpch.h"
#include "BatchRenderer2D.h"
#include "glad/glad.h"

namespace Ovis
{
	const Texture2D* BatchRenderer2D::s_TextureSlots[s_MaxTextureSlots];
	const glm::vec4 BatchRenderer2D::s_QuadCorners[4] = 
	{
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f },
	};

	Renderer2D::Statistics BatchRenderer2D::s_Stats;

	void BatchRenderer2D::Init()
	{
		OV_RENDER_PROFILE_FUNC();
		
		m_QuadVertexBufferBase = new QuadVertex[s_MaxVertices];
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;

		uint32_t* quadIndices = new uint32_t[s_MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_MaxIndices; i += 6)
		{
			quadIndices[i + 0] = 0 + offset;
			quadIndices[i + 1] = 1 + offset;
			quadIndices[i + 2] = 2 + offset;

			quadIndices[i + 3] = 2 + offset;
			quadIndices[i + 4] = 3 + offset;
			quadIndices[i + 5] = 0 + offset;

			offset += 4;
		}

		m_QuadVertexArray = VertexArray::Create();
		m_QuadVertexBuffer = VertexBuffer::Create(s_MaxVertices * sizeof(QuadVertex));
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, s_MaxIndices);
		delete[] quadIndices;

		BufferLayout quadVertexLayout
		{
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" },
			{ ShaderDataType::Float, "a_TilingFactor" }
		};

		m_QuadVertexBuffer->SetLayout(quadVertexLayout);

		m_QuadVertexArray->AddVertexBuffer(m_QuadVertexBuffer);
		m_QuadVertexArray->SetIndexBuffer(indexBuffer);

		m_WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		m_WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		m_WhiteTexture->Bind(0);

		for (size_t i = 0; i < s_MaxTextureSlots; ++i)
			s_TextureSlots[i] = nullptr;

		s_TextureSlots[0] = m_WhiteTexture.get();

		m_StandartShader = Shader::Create("assets/shaders/BatchShader.glsl");
		m_StandartShader->Bind();

		int32_t samplers[s_MaxTextureSlots]; // Temporary buffer to send to GPU as uniform
		for (uint32_t i = 0; i < s_MaxTextureSlots; i++)
			samplers[i] = i;

		m_StandartShader->SetUniform("u_Textures", samplers, s_MaxTextureSlots);
	}

	void BatchRenderer2D::ShutDown()
	{
	}

	void BatchRenderer2D::BeginScene(const OrthographicCamera& camera)
	{
		OV_RENDER_PROFILE_FUNC();

		m_TextureSlotIndex = 0;

		m_StandartShader->Bind();
		m_StandartShader->SetUniform("u_View", camera.GetViewMatrix());
		m_StandartShader->SetUniform("u_Projection", camera.GetProjectionMatrix());

		m_QuadIndexCount = 0;
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
	}

	void BatchRenderer2D::EndScene()
	{
		OV_RENDER_PROFILE_FUNC();

		uint32_t bufferSize = (uint8_t*)m_QuadVertexBufferPtr - (uint8_t*)m_QuadVertexBufferBase;
		m_QuadVertexBuffer->SetData(bufferSize, m_QuadVertexBufferBase);

		Flush();
	}

	void BatchRenderer2D::Flush()
	{
		for (uint32_t i = 0; i <= m_TextureSlotIndex; i++)
			s_TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(m_QuadVertexArray, m_QuadIndexCount);

		s_Stats.DrawCalls++;
	}

	void BatchRenderer2D::FlushAndReset()
	{
		EndScene();

		m_QuadIndexCount = 0;
		m_QuadVertexBufferPtr = m_QuadVertexBufferBase;
		m_TextureSlotIndex = 1;
	}

	void BatchRenderer2D::ResetStats()
	{
		memset(&s_Stats, 0, sizeof(Statistics));
	}

	void BatchRenderer2D::SubmitQuad(const GameEntity& entity, const glm::vec4& color)
	{
		OV_RENDER_PROFILE_FUNC();

		constexpr int quadVertexCount = 4;
		const float textureCoordinates[4][2] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (m_QuadIndexCount >= s_MaxIndices)
			FlushAndReset();

		const glm::mat4& trans = entity.GetTransformationMatrix();

		float whiteTexture = 0.0f;
		
		for (int i = 0; i < quadVertexCount; i++)
		{
			m_QuadVertexBufferPtr->Position = trans * s_QuadCorners[i];
			m_QuadVertexBufferPtr->Color = color;
			m_QuadVertexBufferPtr->TexCoords = { textureCoordinates[i][0], textureCoordinates[i][1] };
			m_QuadVertexBufferPtr->TextureId = whiteTexture;
			m_QuadVertexBufferPtr->TilingFactor = 1.0f;
			m_QuadVertexBufferPtr++;
		}

		m_QuadIndexCount += 6;

		s_Stats.QuadCount++;
	}

	void BatchRenderer2D::SubmitQuad(const GameEntity& entity, const Texture2D& texture, float tilingFactor)
	{
		OV_RENDER_PROFILE_FUNC();

		constexpr int quadVertexCount = 4;
		const float textureCoordinates[4][2] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };

		if (m_QuadIndexCount >= s_MaxIndices)
			FlushAndReset();

		const glm::mat4& trans = entity.GetTransformationMatrix();

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (int i = 1; i <= m_TextureSlotIndex; i++)
		{
			if (*s_TextureSlots[i] == texture)
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			m_TextureSlotIndex++;
			s_TextureSlots[m_TextureSlotIndex] = &texture;
			textureIndex = (float)m_TextureSlotIndex;
		}

		for (int i = 0; i < quadVertexCount; i++)
		{
			m_QuadVertexBufferPtr->Position = trans * s_QuadCorners[i];
			m_QuadVertexBufferPtr->Color = color;
			m_QuadVertexBufferPtr->TexCoords = { textureCoordinates[i][0], textureCoordinates[i][1] };
			m_QuadVertexBufferPtr->TextureId = textureIndex;
			m_QuadVertexBufferPtr->TilingFactor = tilingFactor;
			m_QuadVertexBufferPtr++;
		}

		m_QuadIndexCount += 6;

		s_Stats.QuadCount++;
	}

}

