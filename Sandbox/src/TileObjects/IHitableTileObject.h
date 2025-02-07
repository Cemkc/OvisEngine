#pragma once

class IHitableTileObject
{
public:
	virtual void OnHit(int damage) = 0;
};
