#pragma once

#include "Transform.h"
#include "string"
#include "Ovis/Renderer/Texture.h"

namespace Ovis
{
	class GameEntity
	{
	public:
		GameEntity();
		GameEntity(std::string& name);

		virtual ~GameEntity();

		const Transform& GetTransform() const { return m_Transform; }
		Transform& GetTransform() { return m_Transform; }

		const glm::mat4 GetTransformationMatrix() const;

		void SetName(std::string& name) { name = m_Name; }
		std::string GetName() const { return m_Name; }
		uint32_t GetId() { return m_Id; }

		void AssignParent(GameEntity& parent);
		void AssignChild(GameEntity& child);
		void RemoveChild(GameEntity& child);

		const glm::vec4& GetColor() const { return m_Color; }
		virtual const Texture2D* GetTexture() const { return nullptr; }
	private:
		uint32_t m_Id;

		Transform m_Transform;

		GameEntity* m_Parent;
		std::unordered_map<int, GameEntity*> m_Children;
		
		void Init();

	protected:
		glm::vec4 m_Color;
		std::string m_Name;
	};
}