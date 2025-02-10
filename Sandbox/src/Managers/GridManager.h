#pragma once

#include "Ovis.h"
#include "GridEvent.h"

#include "TileObjects/TileObject.h"
#include "TileObjects/ClickableTileObject.h"
#include "Tile/Tile.h"
#include "Tile/PresentTile.h"
#include "Tile/AbsentTile.h"
#include "TileTypes.h"
#include "TileObjects/RedTile.h"
#include "TileObjects/BlueTile.h"
#include "TileObjects/GreenTile.h"
#include "TileObjects/PurpleTile.h"
#include "TileObjects/YellowTile.h"
#include "TileObjects/RocketTileObject.h"
#include "TileObjects/EmptyTileObject.h"
#include "TileObjects/DuckTileObject.h"

using namespace Ovis;

class GridManager : public Layer
{
public:
	int RunningSequences = 0;

	GridManager();
	static GridManager* s_Instance;
	inline static GridManager& Instance() { return *s_Instance; }
	
	std::vector<std::pair<int, std::function<void(GridEvent&)>>> m_EventCallbacks;
	void AddEventCallback(int id, const std::function<void(GridEvent&)>& callback) { m_EventCallbacks.push_back({ id ,callback }); }
	void RemoveEventCallback(int id) 
	{ 
		auto newEnd = std::remove_if(m_EventCallbacks.begin(), m_EventCallbacks.end(), 
			[id](const std::pair<int, std::function<void(GridEvent&)>>& pair) 
			{ return id == pair.first; });

		m_EventCallbacks.erase(newEnd, m_EventCallbacks.end());
	}

	static constexpr int GridDimension() { return s_GridDimension; }
	static constexpr int TileCount() { return s_GridDimension * s_GridDimension; }

	glm::vec2 GetTileSize() { return m_TileSize; }

	void GetConnectedTiles(int tile, std::list<int>& connectedTiles, std::list<int>& hittableTilesOnEdge, int previousTile = -1);
	Tile* GetTile(glm::ivec2 tilePos);
	Tile* GetTile(int tileNum);

	void OnTileDestroy(Tile* tile);

	inline static glm::vec2 TileIdToPos(int tileId)
	{
		int col = tileId / s_GridDimension;
		int row = tileId % s_GridDimension;
		return glm::vec2(col, row);
	}

private:
	std::shared_ptr<OrthographicCameraController> m_CameraController;

	static constexpr int s_GridDimension = 8;
	glm::vec2 m_TileSize;

	TileObjectType m_StartBoard[s_GridDimension * s_GridDimension];
	std::shared_ptr<Tile> m_TileMap[s_GridDimension][s_GridDimension];

	std::list<TileObject*> m_AnimatedTileObjects;

	double m_ImGuiRefreshTimer = 0;

	int m_PreRunningSequences = 0;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Event& event) override;

	bool OnMouseEvent(MouseButtonPressedEvent& e);
	void OnClick(float posX, float posY);

	void GenerateTileMap();
	void FillEmptyTiles();
	void FillColumn(std::shared_ptr<Tile>& tile, int generatedTileNum = 0);

	std::list<int> GetAdjacentTiles(int tile);
};

