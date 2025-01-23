#include "ovpch.h"
#include "GridManager.h"

#include <random>
#include <ctime>

std::shared_ptr<TileObject> TileTypeToObject(TileObjectType type)
{
	switch (type)
	{
	case TileObjectType::Absent:
		return nullptr;
	case TileObjectType::None:
		return nullptr;
	case TileObjectType::Red:
		return std::make_shared<RedTile>();
	case TileObjectType::Blue:
		return nullptr;
	case TileObjectType::Green:
		return nullptr;
	case TileObjectType::Yellow:
		return nullptr;
	case TileObjectType::Purple:
		return nullptr;
	case TileObjectType::Balloon:
		return nullptr;
	case TileObjectType::Rocket:
		return nullptr;
	case TileObjectType::Duck:
		return nullptr;
	default:
		return nullptr;
	}
}

GridManager* GridManager::s_Instance = nullptr;

GridManager::GridManager()
	:Layer("GridManager")
{
	OV_CORE_ASSERT(!s_Instance, "GridManager already exists!");
	s_Instance = this;
}

void GridManager::OnAttach()
{
	float aspectRatio = Ovis::Application::Get().GetWindow().AspectRatio();
	m_CameraController = std::make_shared<Ovis::OrthographicCameraController>(aspectRatio, true);

	for (int i = 0; i < s_GridDimension * s_GridDimension; i++)
	{
		// Create a random number generator
		std::random_device rd; // Obtain a random number from the system
		std::mt19937 gen(rd()); // Use Mersenne Twister engine
		std::uniform_int_distribution<int> dis(0, 9); // Distribution for the enum range

		// Generate a random number and cast it to the corresponding enum value
		m_StartBoard[i] = static_cast<TileObjectType>(dis(gen));
	}

	GenerateTileMap();
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
	for (int row = 0; row < s_GridDimension; row++)
	{
		for (int col = 0; col < s_GridDimension; col++)
		{
			Renderer2D::Instance().SubmitQuad(*m_TileMap[row][col], { 0.8f, 0.8f, 0.2f, 1.0f });
		}
	}
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

	for (int row = 0; row < s_GridDimension; row++)
	{
		for (int col = 0; col < s_GridDimension; col++)
		{
			entities.push_back(m_TileMap[row][col].get());
		}
	}

	if (Ray::Cast(ray, entities, hitEntities))
	{
		OV_INFO("Ray hit object! ");
		for (GameEntity* entity : hitEntities)
		{
			Tile* tile = static_cast<Tile*>(entity);
			TileObject* tileObj = &tile->GetTileObject();
			// TileObject* tileObj = tile ? &(tile->GetTileObject()) : nullptr;
			if (tileObj->IsInCategory(TileObjectCategory::ClickableCategory))
			{
				ClickableTileObject* clickableObject = dynamic_cast<ClickableTileObject*>(tileObj);
				if (clickableObject)
					clickableObject->OnClick();
			}
			/*if (tileObj != nullptr && tileObj->IsInCategory(TileObjectCategory::ClickableCategory))
			{
				ClickableTileObject* clickableTileObject = static_cast<ClickableTileObject*>(entity);
				clickableTileObject->OnClick();
			}*/
			// OV_INFO("Object name: {0}", entity->GetName());
		}
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
			char r = '0' + col;
			char c = '0' + row;
			std::string name = std::string(1, r) + ", " + c;
			std::shared_ptr<PresentTile> tile = std::make_shared<PresentTile>(name);
			tile->Init(col, row);
			tile->GetTransform().SetPosition(pos);
			tile->GetTransform().SetScale(scale);
			pos.y += tileoffset;

			std::shared_ptr<TileObject> tileObject = nullptr;

			if (col == 0 && row == 0 || col == 0 && row == 1 || col == 0 && row == 2 || col == 1 && row == 0)
			{
				tileObject = std::make_shared<RedTile>();
			}
			else
			{
				tileObject = std::make_shared<BlueTile>();
			}
			
			tile->SetTileObject(tileObject);
			tileObject->SetTile(tile);
			m_TileMap[col][row] = tile;
		}
		pos.x += tileoffset;
		pos.y = startPos.y;
	}
}

void GridManager::GetConnectedTiles(int tile, std::list<int>& connectedTiles, std::list<int>& hittableTilesOnEdge, int previousTile)
{
	std::list<int> adjacentTiles = GetAdjacentTiles(tile);

	if (previousTile == -1)
	{
		connectedTiles.push_back(tile);
	}

	for(int adjacentTile : adjacentTiles)
	{
		if (adjacentTile == previousTile)
		{
			continue;
		}
		TileObjectType selfType = GetTile(tile).GetTileObject().GetTileObjectType();
		TileObjectType adjacentType = GetTile(adjacentTile).GetTileObject().GetTileObjectType();
		if (selfType == adjacentType && adjacentType != TileObjectType::Absent)
		{
			bool listContainsTile = std::find(connectedTiles.begin(), connectedTiles.end(), adjacentTile) != connectedTiles.end();
			if (!listContainsTile)
			{
				connectedTiles.push_back(adjacentTile);
				GetConnectedTiles(adjacentTile, connectedTiles, hittableTilesOnEdge, tile);
			}
		}
		else if (GetTile(adjacentTile).GetTileObject().IsInCategory(TileObjectCategory::HitableTileObject))
		{
			hittableTilesOnEdge.push_back(adjacentTile);
		}
	}

	return;
}

std::list<int> GridManager::GetAdjacentTiles(int tile)
{
	std::list<int> adjacentTiles;

	// Calculate row and column of the given tile
	int row = tile % s_GridDimension;
	int col = tile / s_GridDimension;

	// Check above (row + 1)
	if (row + 1 < s_GridDimension && GetTile(tile + 1).GetTileObject().GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile + 1);
	}

	// Check below (row - 1)
	if (row - 1 >= 0 && GetTile(tile - 1).GetTileObject().GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile - 1);
	}

	// Check right (col + 1)
	if (col + 1 < s_GridDimension && GetTile(tile + s_GridDimension).GetTileObject().GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile + s_GridDimension);
	}

	// Check left (col - 1)
	if (col - 1 >= 0 && GetTile(tile - s_GridDimension).GetTileObject().GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile - s_GridDimension);
	}

	return adjacentTiles;
}

Tile& GridManager::GetTile(glm::ivec2 tilePos)
{
	if (tilePos.x >= s_GridDimension || tilePos.y >= s_GridDimension ||
		tilePos.x < 0 || tilePos.y < 0)
		OV_ASSERT("Tile number out of bounds!");

	return *m_TileMap[tilePos.x][tilePos.y];
}

Tile& GridManager::GetTile(int tileNum)
{
	if (tileNum >= s_GridDimension * s_GridDimension || tileNum < 0)
		OV_ASSERT("Tile number out of bounds!");

	return *m_TileMap[tileNum / s_GridDimension][tileNum % s_GridDimension];
}
