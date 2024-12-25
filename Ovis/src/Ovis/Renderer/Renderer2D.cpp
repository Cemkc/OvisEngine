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
		std::shared_ptr<ShaderLibrary> ShaderLibrary;
		glm::mat4 ViewMatrix;
		glm::mat4 ProjectionMatrix;
	};

	static Renderer2DStorage* s_Storage;

	void Renderer2D::Init()
	{
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

		s_Storage->ShaderLibrary = std::make_shared<ShaderLibrary>();
		s_Storage->ShaderLibrary->Load("assets/shaders/FlatColor.glsl");
		auto textureShader = s_Storage->ShaderLibrary->Load("assets/shaders/Texture.glsl");
		textureShader->Bind();
		textureShader->SetUniform("u_Texture", 0);
	}

	void Renderer2D::ShutDown()
	{
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
		std::shared_ptr<Shader> shader = s_Storage->ShaderLibrary->Get("FlatColor");
		shader->Bind();
	
		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		shader->SetUniform("u_model", model);
		shader->SetUniform("u_view", s_Storage->ViewMatrix);
		shader->SetUniform("u_projection", s_Storage->ProjectionMatrix);

		shader->SetUniform("u_Color", color);

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const Transform& transform, const Texture2D& texture)
	{
		std::shared_ptr<Shader> shader = s_Storage->ShaderLibrary->Get("Texture");
		shader->Bind();

		glm::mat4 model(1.0f);
		model = glm::translate(model, transform.Position);
		model = glm::scale(model, transform.Scale);
		model = glm::rotate(model, glm::radians(transform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		shader->SetUniform("u_model", model);
		shader->SetUniform("u_view", s_Storage->ViewMatrix);
		shader->SetUniform("u_projection", s_Storage->ProjectionMatrix);

		texture.Bind();

		s_Storage->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Storage->QuadVertexArray);
	}

}

