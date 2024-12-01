#pragma once
#include "Ovis/Layer.h"

#include "Ovis/Events/ApplicationEvent.h"
#include "Ovis/Events/MouseEvent.h"
#include "Ovis/Events/KeyEvent.h"

namespace Ovis {
	class ImGuiLayer : public Layer
	{
	private:
		float m_Time = 0.0f;
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();

	};
}


