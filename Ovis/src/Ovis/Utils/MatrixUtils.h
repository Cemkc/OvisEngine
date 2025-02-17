#pragma once

#include <glm/glm.hpp>

namespace Ovis
{
	class Utils
	{
	public:
		// Helper function to format a float with a specified precision
		static std::string FormatFloat(float value, int precision = 3);

		// Function to convert glm::mat4 to string
		static std::string Mat4ToString(const glm::mat4& matrix);
	};
}