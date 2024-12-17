#pragma once

#include <chrono>

#include "Events/Event.h"

namespace Ovis
{
	class Time
	{
	private:
		static std::chrono::time_point<std::chrono::high_resolution_clock> m_FrameBeginTime;
		static std::chrono::time_point<std::chrono::high_resolution_clock> m_AppBeginTime;
		static std::chrono::duration<double> Time::m_DeltaTime;
	public:
		Time();

		static double GetTime();
		static const double DeltaTime(); // Returns delta time in seconds

		void OnApplicationEvent(Event& event);

	private:
		void OnFrameBegin();
		void OnFrameEnd();
	};
}