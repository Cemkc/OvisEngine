#include "ovpch.h"
#include "BatchRenderer2D.h"

namespace Ovis
{

	const Texture2D* BatchRenderer2D::s_TextureSlots[s_MaxTextureSlots];

	void BatchRenderer2D::Init()
	{
		OV_RENDER_PROFILE_FUNC();
		
		m_QuadVertexBufferBase = new Vertex[s_MaxVertices];
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
		m_QuadVertexBuffer = VertexBuffer::Create(s_MaxVertices * sizeof(Vertex));
		std::shared_ptr<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, s_MaxIndices);
		delete[] quadIndices;

		BufferLayout quadVertexLayout
		{
			{ ShaderDataType::Float3, "a_Pos" },
			{ ShaderDataType::Float4, "a_Color" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float, "a_TexIndex" }
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

		m_StandartShader = Shader::Create("assets/shaders/Texture.glsl");
		m_StandartShader->Bind();

		int32_t samplers[s_MaxTextureSlots];
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
		/*m_QuadVertexArray->Bind();
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);*/

		for (uint32_t i = 0; i <= m_TextureSlotIndex; i++)
			s_TextureSlots[i]->Bind(i);

		RenderCommand::DrawIndexed(m_QuadVertexArray, m_QuadIndexCount);
	}

	void BatchRenderer2D::SubmitQuad(const Transform& transform, const glm::vec4& color)
	{
		OV_RENDER_PROFILE_FUNC();

		float whiteTexture = 0.0f;

		float size = 0.05f; // (Half Size)

		float px = transform.Position.x;
		float py = transform.Position.y;
		float pz = transform.Position.z;

		float sx = transform.Position.x;
		float sy = transform.Position.y;

		m_QuadVertexBufferPtr->Position = { px - size, py - size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 0.0f };
		m_QuadVertexBufferPtr->TextureId = whiteTexture;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = { px + size, py - size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 0.0f };
		m_QuadVertexBufferPtr->TextureId = whiteTexture;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = { px + size, py + size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 1.0f };
		m_QuadVertexBufferPtr->TextureId = whiteTexture;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = { px - size, py + size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 1.0f };
		m_QuadVertexBufferPtr->TextureId = whiteTexture;
		m_QuadVertexBufferPtr++;

		m_QuadIndexCount += 6;
	}

	void BatchRenderer2D::SubmitQuad(const Transform& transform, const Texture2D& texture, float tilingFactor)
	{
		OV_RENDER_PROFILE_FUNC();

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

		float size = 0.5f; // (Half Size)

		float px = transform.Position.x;
		float py = transform.Position.y;
		float pz = transform.Position.z;

		float sx = transform.Position.x;
		float sy = transform.Position.y;

		m_QuadVertexBufferPtr->Position = { px - size, py - size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 0.0f };
		m_QuadVertexBufferPtr->TextureId = textureIndex;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = { px + size, py - size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 0.0f };
		m_QuadVertexBufferPtr->TextureId = textureIndex;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = { px + size, py + size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 1.0f, 1.0f };
		m_QuadVertexBufferPtr->TextureId = textureIndex;
		m_QuadVertexBufferPtr++;

		m_QuadVertexBufferPtr->Position = { px - size, py + size, pz };
		m_QuadVertexBufferPtr->Color = color;
		m_QuadVertexBufferPtr->TexCoords = { 0.0f, 1.0f };
		m_QuadVertexBufferPtr->TextureId = textureIndex;
		m_QuadVertexBufferPtr++;

	}

}

