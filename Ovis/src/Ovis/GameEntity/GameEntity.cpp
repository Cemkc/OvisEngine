#include "ovpch.h"
#include "GameEntity.h";

namespace Ovis
{
	GameEntity::GameEntity()
		:m_Transform(Transform())
	{ 
	}

	GameEntity::GameEntity(std::string& name)
		:m_Transform(Transform()), m_Name(name)
	{

	}
}