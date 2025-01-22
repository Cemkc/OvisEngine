#pragma once

#include "Transform.h"
#include "string"

namespace Ovis
{
	class GameEntity
	{
	public:
		GameEntity();
		GameEntity(std::string& name);

		const Transform& GetTransform() const { return m_Transform; }
		Transform& GetTransform() { return m_Transform; }

		void SetName(std::string& name) { name = m_Name; }
		std::string GetName() { return m_Name; }
	private:
		Transform m_Transform;
		std::string m_Name;
	};
}