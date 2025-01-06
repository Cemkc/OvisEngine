#include "ovpch.h"
#include "Renderer2D.h"

#include "Ovis/Core/Application.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Ovis
{
	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<Shader> StandartShader;
		std::shared_ptr<Texture2D> WhiteTexture;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	};

	static Renderer2DStorage* s_Storage;

	void Renderer2D::Init()
	{
		OV_RENDER_PROFILE_FUNC();

		s_Storage = new Renderer2DStorage();

		float squareVertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
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
			0, 1, 2, 2, 3, 0
		};

		std::shared_ptr<IndexBuffer> squareIndexBuffer;
		squareIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

		s_Storage->QuadVertexArray = VertexArray::Create();
		s_Storage->QuadVertexArray->AddVertexBuffer(squareVertexBuffer);
		s_Storage->QuadVertexArray->SetIndexBuffer(squareIndexBuffer);

		s_Storage->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Storage->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Storage->StandartShader = Shader::Create("assets/shaders/Texture.glsl");
		s_Storage->StandartShader->Bind();
		s_Storage->StandartShader->SetUniform("u_Texture", 0);
	}

	void Renderer2D::ShutDown()
	{
		OV_RENDER_PROFILE_FUNC();

		delete s_Storage;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Storage->ViewMatrix = camera.GetViewMatrix();
		s_Storage->ProjectionMatrix = camera.GetProjectionMatrix();
	}

	void Renderer2D::EndScene()
	{
	}

	void Renderer2D::DrawQuad(const Transform& transform, const glm::vec4& color)
	{
		OV_RENDER_PROFILE_FUNC();

		s_Storage->StandartShader->SetUniform("u_Color", color);
		s_Storage->WhiteTexture->Bind();

		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		if(transform.Rotation.x != 0 || transform.Rotation.y != 0 || transform.Rotation.z != 0) //  This saves us from calculating a rotation matrix which can be costly if no rotation is applied
			model = glm::rotate(model, glm::radians(transform.Rotation.z), { 0.0f, 0.0f, 1.0f });

		s_Storage->StandartShader->SetUniform("u_model", model);
		s_Storage->StandartShader->SetUniform("u_view", s_Storage->ViewMatrix);
		s_Storage->StandartShader->SetUniform("u_projection", s_Storage->ProjectionMatrix);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const Transform& transform, const Texture2D& texture, float tilingFactor)
	{
		OV_RENDER_PROFILE_FUNC();

		s_Storage->StandartShader->Bind();
		s_Storage->StandartShader->SetUniform("u_Color", glm::vec4(1.0f));
		s_Storage->StandartShader->SetUniform("u_TilingFactor", tilingFactor);
		texture.Bind();

		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		if (transform.Rotation.x != 0 || transform.Rotation.y != 0 || transform.Rotation.z != 0) //  This saves us from calculating a rotation matrix which can be costly if no rotation is applied
			model = glm::rotate(model, glm::radians(transform.Rotation.z), { 0.0f, 0.0f, 1.0f });

		s_Storage->StandartShader->SetUniform("u_model", model);
		s_Storage->StandartShader->SetUniform("u_view", s_Storage->ViewMatrix);
		s_Storage->StandartShader->SetUniform("u_projection", s_Storage->ProjectionMatrix);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
	}

}

