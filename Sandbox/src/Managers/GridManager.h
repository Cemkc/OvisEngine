#pragma once

#include "Ovis.h"

using namespace Ovis;

class GridManager : public Layer
{
public:
	GridManager();
	static const int s_GridDimension;

private:

	std::shared_ptr<OrthographicCameraController> m_CameraController;
	GameEntity m_Tile;
	std::vector<GameEntity> m_Tiles;
	std::shared_ptr<Texture2D> m_FrogTexture;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Event& event) override;

	bool OnMouseEvent(MouseButtonPressedEvent& e);
	void OnClick(float posX, float posY);

	void GenerateTileMap();
};

