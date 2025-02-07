#pragma once

#include "Ovis.h"
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

using namespace Ovis;

class GridManager : public Layer
{
public:
	GridManager();
	static GridManager* s_Instance;

	int m_RunningSequences = 0;

	static constexpr int GridDimension() { return s_GridDimension; }
	static constexpr int TileNumber() { return s_GridDimension * s_GridDimension; }

	glm::vec2 GetTileSize() { return m_TileSize; }

	inline static glm::vec2 TileIdToPos(int tileId)
	{
		int col = tileId / s_GridDimension;
		int row = tileId % s_GridDimension;
		return glm::vec2(col, row);
	}

	inline static GridManager& Instance() { return *s_Instance; }

	void GetConnectedTiles(int tile, std::list<int>& connectedTiles, std::list<int>& hittableTilesOnEdge, int previousTile = -1);
	Tile* GetTile(glm::ivec2 tilePos);
	Tile* GetTile(int tileNum);

	void OnTileDestroy(Tile* tile);

private:
	std::shared_ptr<OrthographicCameraController> m_CameraController;

	static constexpr int s_GridDimension = 8;
	glm::vec2 m_TileSize;

	TileObjectType m_StartBoard[s_GridDimension * s_GridDimension];
	std::shared_ptr<Tile> m_TileMap[s_GridDimension][s_GridDimension];

	std::list<TileObject*> m_AnimatedTileObjects;

	double m_ImGuiRefreshTimer = 0;

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

