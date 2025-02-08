#include "ovpch.h"
#include "GridManager.h"

#include <random>
#include <ctime>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "AnimationManager.h"

std::shared_ptr<TileObject> TileTypeToObject(TileObjectType type)
{
	switch (type)
	{
	case TileObjectType::Absent:
		return nullptr;
	case TileObjectType::None:
		return std::make_shared<EmptyTileObject>();
	case TileObjectType::Red:
		return std::make_shared<RedTile>();
	case TileObjectType::Blue:
		return std::make_shared<BlueTile>();
	case TileObjectType::Green:
		return std::make_shared<GreenTile>();;
	case TileObjectType::Yellow:
		return std::make_shared<YellowTile>();;
	case TileObjectType::Purple:
		return std::make_shared<PurpleTile>();;
	case TileObjectType::Balloon:
		return nullptr;
	case TileObjectType::Rocket:
		return std::make_shared<RocketTileObject>();
	case TileObjectType::Duck:
		return std::make_shared<DuckTileObject>();
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
	for (int row = 0; row < s_GridDimension; row++)
	{
		for (int col = 0; col < s_GridDimension; col++)
		{
			std::shared_ptr<TileObject> tileObj = m_TileMap[row][col]->GetTileObject();
			if (tileObj && tileObj->GetTileObjectType() == TileObjectType::None && RunningSequences == 0)
			{
				FillEmptyTiles();
				for (const auto& [id, func] : m_EventCallbacks)
				{
					FillEndEvent e;
					func(e);
				}
			}
		}
	}

	m_CameraController->OnUpdate();

	RenderCommand::SetClearColor(glm::vec4(0.1f, 0.62f, 0.443f, 1.0f));
	Ovis::RenderCommand::Clear();

	Ovis::Renderer2D::Instance().ResetStats();

	Renderer2D::Instance().BeginScene(m_CameraController->GetCamera());

	for (int row = 0; row < s_GridDimension; row++)
	{
		for (int col = 0; col < s_GridDimension; col++)
		{
			Renderer2D::Instance().SubmitQuad(*m_TileMap[row][col], { 0.2f, 0.2f, 0.2f, 1.0f });

			std::shared_ptr<TileObject> tileObj = m_TileMap[row][col]->GetTileObject();
			Renderer2D::Instance().SubmitQuad(*tileObj, tileObj->GetColor());

			if (tileObj && tileObj->GetTileObjectType() == TileObjectType::Rocket)
			{
				RocketTileObject* rocket = dynamic_cast<RocketTileObject*>(tileObj.get());
				rocket->OnUpdate();
			}
		}
	}

	Renderer2D::Instance().EndScene();
}

int frameRate = 0;
float frameTime = 0;

void GridManager::OnImGuiRender()
{
	OV_PROFILE_FUNC();

	ImGui::Begin("Metrics Panel");

	ImGui::Text("Performance Metrics:");
	if (Ovis::Time::GetTime() - m_ImGuiRefreshTimer > 1)
	{
		frameRate = (int)(1 / Ovis::Time::DeltaTime());
		frameTime = Ovis::Time::DeltaTime() * 1000;
		m_ImGuiRefreshTimer = Ovis::Time::GetTime();
	}

	ImGui::Text("Frame Rate: %d fps", frameRate);
	ImGui::Text("Frame Time: %f ms", frameTime);

	auto stats = Ovis::Renderer2D::Instance().GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	if(ImGui::Button("Fill Empty Tiles"))
		FillEmptyTiles();

	std::string tileString;
	for (int col = 0; col < s_GridDimension; col++)
	{
		for (int row = 0; row < s_GridDimension; row++)
		{
			std::ostringstream oss;
			oss << m_TileMap[col][row]->GetTileObject().get();
			tileString += m_TileMap[col][row]->GetName() + ": " + m_TileMap[col][row]->GetTileObject()->GetName() + " Address: " + oss.str() + "\n";
		}
	}

	// Make the buffer large enough to hold the long text
	char buffer[2048];  // Adjust size depending on your use case
	std::strncpy(buffer, tileString.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0'; // Ensure null termination

	// Use InputText with multiline and readonly flags
	ImGui::InputTextMultiline("##CopyField", buffer, sizeof(buffer),
		ImVec2(-1.0f, ImGui::GetTextLineHeight() * 40),  // Adjust the height of the input box
		ImGuiInputTextFlags_ReadOnly);

	ImGui::End();
}

void GridManager::OnEvent(Event& event)
{
	m_CameraController->OnEvent(event);

	EventDispatcher dispatcher(event);
	dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&GridManager::OnMouseEvent, this, std::placeholders::_1));
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
			std::shared_ptr<TileObject> tileObj = tile->GetTileObject();
			// TileObject* tileObj = tile ? &(tile->GetTileObject()-> : nullptr;
			if (tileObj->IsInCategory(TileObjectCategory::ClickableCategory))
			{
				std::shared_ptr<ClickableTileObject> clickableObject = std::dynamic_pointer_cast<ClickableTileObject>(tileObj);
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

	float tileSpacing = 0.02f;
	m_TileSize.x = (2.0f - horizontalMargin * 2.0f) / s_GridDimension;
	m_TileSize.y = (2.0f - verticalMargin * 2.0f) / s_GridDimension;

	glm::vec3 scale = { m_TileSize.x * 0.8f, m_TileSize.y * 0.8f, 1.0f };

	glm::vec3 startPos = { -1.0f + horizontalMargin + m_TileSize.x / 2, -1.0f + verticalMargin + m_TileSize.y / 2, 0.0f };
	glm::vec3 pos = startPos;

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
			pos.y += m_TileSize.y;

			std::shared_ptr<TileObject> tileObject = nullptr;

			if (col == 0 && row == 4 || col == 4 && row == 4 /*|| col == 1 && row == 4 || col == 0 && row == 2 || col == 1 && row == 0*/)
			{
				tileObject = TileTypeToObject(TileObjectType::Rocket);
			}
			else
			{
				tileObject = TileTypeToObject(TileObjectType::Blue);
			}

			tile->SetTileObject(tileObject);
			tileObject->SetTile(tile.get());

			glm::vec3 pos = tile->GetTransform().GetPosition();
			pos.z = 0.1f;
			tileObject->GetTransform().SetPosition(pos);
			tileObject->GetTransform().SetScale(glm::vec3(scale.x * 0.8f, scale.y * 0.8f, 1.0f));

			m_TileMap[col][row] = tile;
		}
		pos.x += m_TileSize.x;
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
		TileObjectType selfType = GetTile(tile)->GetTileObject()->GetTileObjectType();
		TileObjectType adjacentType = GetTile(adjacentTile)->GetTileObject()->GetTileObjectType();
		if (selfType == adjacentType && adjacentType != TileObjectType::Absent)
		{
			bool listContainsTile = std::find(connectedTiles.begin(), connectedTiles.end(), adjacentTile) != connectedTiles.end();
			if (!listContainsTile)
			{
				connectedTiles.push_back(adjacentTile);
				GetConnectedTiles(adjacentTile, connectedTiles, hittableTilesOnEdge, tile);
			}
		}
		else if (GetTile(adjacentTile)->GetTileObject()->IsInCategory(TileObjectCategory::HitableTileObject))
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
	if (row + 1 < s_GridDimension && GetTile(tile + 1)->GetTileObject()->GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile + 1);
	}

	// Check below (row - 1)
	if (row - 1 >= 0 && GetTile(tile - 1)->GetTileObject()->GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile - 1);
	}

	// Check right (col + 1)
	if (col + 1 < s_GridDimension && GetTile(tile + s_GridDimension)->GetTileObject()->GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile + s_GridDimension);
	}

	// Check left (col - 1)
	if (col - 1 >= 0 && GetTile(tile - s_GridDimension)->GetTileObject()->GetTileObjectType() != TileObjectType::Absent)
	{
		adjacentTiles.push_back(tile - s_GridDimension);
	}

	return adjacentTiles;
}

void GridManager::FillEmptyTiles()
{
	for (int col = 0; col < s_GridDimension; col++)
	{
		for (int row = 0; row < s_GridDimension; row++)
		{
			std::shared_ptr<Tile> tile = m_TileMap[col][row];
			if (tile->GetTileObject()->GetTileObjectType() == TileObjectType::None)
			{
				OV_INFO("Detected empty tile starting recursive algorithm. Empty tile is: ({0}, {1})", tile->GetTilePos().x, tile->GetTilePos().y);
				FillColumn(tile); // This will recursively fill the empty tiles.
				break;
			}
		}
	}
}

void GridManager::FillColumn(std::shared_ptr<Tile>& tile, int generatedTileNum)
{
	TileObject* tileObj = tile->GetTileObject().get();
	if (tileObj->GetTileObjectType() == TileObjectType::None)
	{
		if (tileObj->GetTileObjectType() == TileObjectType::Absent)
		{
			if (tile->GetTilePos().y + 1 >= s_GridDimension) return;
			else FillColumn(m_TileMap[tile->GetTilePos().x][tile->GetTilePos().y + 1]);
		}

		OV_INFO("Recursive algorithm started tile ({0}, {1})) seems to be empty attempting to fill. ", tile->GetTilePos().x, tile->GetTilePos().y);
		bool foundTileInGrid = false;
		for (int row = tile->GetTilePos().y + 1; row < s_GridDimension; row++)
		{
			std::shared_ptr<Tile> tileAbove = m_TileMap[tile->GetTilePos().x][row];
			std::shared_ptr<TileObject> tileObjAbove = m_TileMap[tile->GetTilePos().x][row]->GetTileObject();
			if (tileObjAbove->GetTileObjectType() != TileObjectType::None && tileObjAbove->GetTileObjectType() != TileObjectType::Absent) // Change with tile.CanFall()
			{
				foundTileInGrid = true;
				OV_INFO("Found Tile Object in the column!");
				AnimationManager::Instance().MoveObject(*tileObjAbove, tile->GetTransform().GetPosition());
				//StartCoroutine(FallToPosition(tileObjAbove, tile));
				glm::vec3 pos = { tile->GetTransform().GetPosition().x, tile->GetTransform().GetPosition().y, 0.1f };
				//tileObjAbove->GetTransform().SetPosition(pos);
				tile->SetTileObject(tileObjAbove);

				std::shared_ptr<TileObject> emptyTileObj = TileTypeToObject(TileObjectType::None);
				tileAbove->SetTileObject(emptyTileObj);
				break;
			}
		}

		if (!foundTileInGrid)
		{
			// Create a random number generator
			std::random_device rd; // Obtain a random number from the system
			std::mt19937 gen(rd()); // Use Mersenne Twister engine
			std::uniform_int_distribution<int> dis(2, 6); // Distribution for the enum range

			// Generate a random number and cast it to the corresponding enum value
			TileObjectType type = static_cast<TileObjectType>(dis(gen));
			OV_INFO((int)type);

			std::shared_ptr<TileObject> tileObject = TileTypeToObject(type);
			generatedTileNum++;
			glm::vec3 pos = { tile->GetTransform().GetPosition().x , GetTile(0)->GetTransform().GetPosition().y + (m_TileSize.y * (s_GridDimension - 1)) + (m_TileSize.y * generatedTileNum), 0.1f};
			tileObject->GetTransform().SetPosition(pos);
			glm::vec3 scale = tile->GetTransform().GetScale();
			scale = scale * 0.8f;
			scale.z = 1.0f;
			tileObject->GetTransform().SetScale(scale);
			tile->SetTileObject(tileObject);
			AnimationManager::Instance().MoveObject(*tileObject, tile->GetTransform().GetPosition());
			//StartCoroutine(FallToPosition(tileObject, tile));
		}

	}

	if (tile->GetTilePos().y + 1 >= s_GridDimension) return;
	else FillColumn(m_TileMap[tile->GetTilePos().x][tile->GetTilePos().y + 1], generatedTileNum);

}

void GridManager::OnTileDestroy(Tile* tile)
{
	std::shared_ptr<TileObject> emptyTileObj = TileTypeToObject(TileObjectType::None);
	glm::vec3 emptyTilePos = { tile->GetTransform().GetPosition().x, tile->GetTransform().GetPosition().y, 0.1f };
	emptyTileObj->GetTransform().SetPosition(emptyTilePos);
	glm::vec3 scale = tile->GetTransform().GetScale();
	scale = scale * 0.8f;
	scale.z = 1.0f;
	emptyTileObj->GetTransform().SetScale(scale);
	tile->SetTileObject(emptyTileObj);
}

Tile* GridManager::GetTile(glm::ivec2 tilePos)
{
	if (tilePos.x >= s_GridDimension || tilePos.y >= s_GridDimension ||
		tilePos.x < 0 || tilePos.y < 0)
		return nullptr;

	return m_TileMap[tilePos.x][tilePos.y].get();
}

Tile* GridManager::GetTile(int tileNum)
{
	if (tileNum >= s_GridDimension * s_GridDimension || tileNum < 0)
		return nullptr;

	return m_TileMap[tileNum / s_GridDimension][tileNum % s_GridDimension].get();
}
