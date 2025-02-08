#pragma once

#include "TileObject.h"

class DuckTileObject : public TileObject
{
public:
	DuckTileObject();
	~DuckTileObject();

private:
	int m_DuckId;
	static bool s_DuckIdArray[];

	void OnGridEvent(GridEvent& event);
	void OnFillEnd();
};
