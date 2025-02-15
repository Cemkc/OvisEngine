#pragma once

enum class TileObjectType
{
	Absent = 0,
	None,
	Pearl,
	Shell,
	Seaweed,
	Tentacle,
	Star,
	Bottle,
	Harpoon,
	Anchor
};

enum TileObjectCategory
{
	Absent = 0,
	None = 1 << 0,
	HitableTileObject = 1 << 1,
	ConstantTileObject = 1 << 2,
	ClickableCategory = 1 << 3,
	FallableTileObject = 1 << 4,
	MatchSensitiveObject = 1 << 5,
	AudibleTileObject = 1 << 6,
	ParticleEmittingTileobject = 1 << 7
};
