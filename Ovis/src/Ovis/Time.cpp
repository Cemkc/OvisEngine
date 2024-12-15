#include "ovpch.h"

#include "Time.h"
#include "Application.h"

namespace Ovis
{
	std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_FrameBeginTime;
	std::chrono::time_point<std::chrono::high_resolution_clock> Time::m_AppBeginTime;
	std::chrono::duration<double> Time::m_DeltaTime;

	Time::Time()
	{
		Application::Get().AddEventCallback(std::bind(&Time::OnApplicationEvent, this, std::placeholders::_1));

		m_AppBeginTime = std::chrono::high_resolution_clock::now();
		m_FrameBeginTime = m_AppBeginTime;
	}

	void Time::OnApplicationEvent(Event& event)
	{
		if(event.GetEventType() == EventType::AppRender)
		{
			AppRenderEvent* appRenderEvent = static_cast<AppRenderEvent*>(&event);
			switch (appRenderEvent->GetState())
			{
			case LoopState::Begin:
				OnFrameBegin();
				break;
			case LoopState::End:
				OnFrameEnd();
				break;
			}
		}
	}

	void Time::OnFrameBegin()
	{
		m_FrameBeginTime = std::chrono::high_resolution_clock::now();
	}

	void Time::OnFrameEnd()
	{
		m_DeltaTime = std::chrono::high_resolution_clock::now() - m_FrameBeginTime;
	}

	double Time::GetTime()
	{
		return std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - m_AppBeginTime).count();
	}

	const double Time::GetDeltaTime()
	{
		m_DeltaTime = std::chrono::high_resolution_clock::now() - m_FrameBeginTime;
		return m_DeltaTime.count();
	}
}