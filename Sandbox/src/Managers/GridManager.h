#pragma once

#include "Ovis.h"
#include "GridEvent.h"

#include "TileObjects/TileObject.h"
#include "TileObjects/ClickableTileObject.h"
#include "Tile/Tile.h"
#include "Tile/PresentTile.h"
#include "Tile/AbsentTile.h"
#include "TileTypes.h"

#include "TileObjects/MatchObjects/Pearl.h"
#include "TileObjects/MatchObjects/Shell.h"
#include "TileObjects/MatchObjects/Seaweed.h"
#include "TileObjects/MatchObjects/Star.h"
#include "TileObjects/MatchObjects/Tentacle.h"

#include "TileObjects/Harpoon.h"
#include "TileObjects/EmptyTileObject.h"
#include "TileObjects/Anchor.h"
#include "TileObjects/Bottle.h"

using namespace Ovis;

class GridManager : public Layer
{
public:
	int RunningSequences = 0;

	GridManager();
	static GridManager* s_Instance;
	inline static GridManager& Instance() { return *s_Instance; }

	static constexpr int GridDimension() { return s_GridDimension; }
	static constexpr int TileCount() { return s_GridDimension * s_GridDimension; }

	glm::vec2 GetTileSize() { return m_TileSize; }

	void GetConnectedTiles(int tile, std::list<int>& connectedTiles, std::list<int>& hittableTilesOnEdge, int previousTile = -1);

	void SetTile(Tile& tile, std::shared_ptr<TileObject>& tileObject);
	void SetTile(Tile& tile, TileObjectType type);
	void SetTile(int tileId, TileObjectType type);

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

	int m_PreRunningSequences = 0;

	std::shared_ptr<GameEntity> m_BackgroundEntity;
	std::shared_ptr<Texture2D> m_BackgroundImage;

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

