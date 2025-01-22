#include "GridManager.h"

const int GridManager::s_GridDimension = 6;

GridManager::GridManager()
	:Layer("GridManager")
{
}

void GridManager::OnAttach()
{
	float aspectRatio = Ovis::Application::Get().GetWindow().AspectRatio();
	m_CameraController = std::make_shared<Ovis::OrthographicCameraController>(aspectRatio, true);

	GenerateTileMap();

	m_FrogTexture = Ovis::Texture2D::Create("assets/textures/Frog.png");

	m_Tile.GetTransform().SetScale(glm::vec3(0.5f, 0.5f, 1.0f));
	m_Tile.GetTransform().SetPosition(glm::vec3(0.5f, 0.5f, 0.0f));
}

void GridManager::OnDetach()
{
}

void GridManager::OnUpdate()
{
	m_CameraController->OnUpdate();

	RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	Ovis::RenderCommand::Clear();

	Renderer2D::Instance().BeginScene(m_CameraController->GetCamera());

	//glm::vec3 pos = m_Tile.GetTransform().GetPosition();
	//pos.y += Time::DeltaTime() * 1.0f;
	//m_Tile.GetTransform().SetPosition(pos);

	// Renderer2D::Instance().SubmitQuad(m_Tile, *m_FrogTexture.get(), 1.0f);
	for (const GameEntity& tile : m_Tiles)
		Renderer2D::Instance().SubmitQuad(tile, { 0.8f, 0.8f, 0.2f, 1.0f });
	//Renderer2D::Instance().SubmitQuad(m_Tile, { 0.8f, 0.8f, 0.2f, 1.0f });

	Renderer2D::Instance().EndScene();
}

void GridManager::OnClick(float posX, float posY)
{
	OV_INFO("Mouse clicked at position: ({0}, {1})", posX, posY);
	glm::vec2 worldPos = Camera::ScreenToWorldPoint(posX, posY);
	OV_INFO("Mouse world position: ({0}, {1})", worldPos.x, worldPos.y);
	Ray ray({worldPos.x, worldPos.y, 1.0f}, {0.0f, 0.0f, -1.0f});
	std::vector<GameEntity*> hitEntities;
	std::vector<GameEntity*> entities;

	for (GameEntity& tile : m_Tiles)
	{
		entities.push_back(&tile);
	}

	if (Ray::Cast(ray, entities, hitEntities))
	{
		OV_INFO("Ray hit object! ");
		for(GameEntity* entity : hitEntities)
			OV_INFO("Object name: {0}", entity->GetName());
	}
}

void GridManager::OnEvent(Event& event)
{
	m_CameraController->OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&GridManager::OnMouseEvent, this, std::placeholders::_1));
}

bool GridManager::OnMouseEvent(MouseButtonPressedEvent& e)
{
	if (e.GetMouseButton() == 0)
	{
		auto [x, y] = Input::GetMousePos();
		OnClick(x, y);
	}

	return true;
}

void GridManager::GenerateTileMap()
{
	float horizontalMargin = 0.2f;
	float verticalMargin = 0.2f;

	float tileoffset = 0.14f;

	glm::vec3 startPos = { -1.0f + horizontalMargin, -1.0f + verticalMargin, 0.0f };
	glm::vec3 pos = startPos;
	float scaleFactor = (2.0f - horizontalMargin * 2) / s_GridDimension;
	glm::vec3 scale = { scaleFactor - tileoffset, scaleFactor - tileoffset, 1.0f };

	for (int col = 0; col < s_GridDimension; col++)
	{
		for (int row = 0; row < s_GridDimension; row++)
		{
			char r = '0' + row;
			char c = '0' + col;
			std::string name = std::string(1, r) + ", " + c;
			GameEntity tile(name);
			tile.GetTransform().SetPosition(pos);
			tile.GetTransform().SetScale(scale);
			pos.y += tileoffset;
			m_Tiles.push_back(tile);
		}
		pos.x += tileoffset;
		pos.y = startPos.y;
	}
}
