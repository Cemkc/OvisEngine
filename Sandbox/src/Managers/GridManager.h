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

using namespace Ovis;

class GridManager : public Layer
{
public:
	GridManager();
	static GridManager* s_Instance;
	static const int s_GridDimension = 6;

	inline static GridManager& Instance() { return *s_Instance; }

	void GetConnectedTiles(int tile, std::list<int>& connectedTiles, std::list<int>& hittableTilesOnEdge, int previousTile = -1);
	Tile& GetTile(glm::ivec2 tilePos);
	Tile& GetTile(int tileNum);

private:
	std::shared_ptr<OrthographicCameraController> m_CameraController;

	TileObjectType m_StartBoard[s_GridDimension * s_GridDimension];
	std::shared_ptr<Tile> m_TileMap[s_GridDimension][s_GridDimension];

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Event& event) override;

	bool OnMouseEvent(MouseButtonPressedEvent& e);
	void OnClick(float posX, float posY);

	void GenerateTileMap();

	std::list<int> GetAdjacentTiles(int tile);
};

