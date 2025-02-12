#pragma once

#include "TileObject.h"

class DuckTileObject : public TileObject
{
public:
	DuckTileObject();
	~DuckTileObject();

	void OnFillEnd();
private:
	std::function<void()> m_OnFillEndCallback;
	int m_CallBackId;
};
