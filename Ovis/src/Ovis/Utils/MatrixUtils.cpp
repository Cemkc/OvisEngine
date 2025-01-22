#include "ovpch.h"
#include "MatrixUtils.h"

#include <string.h>
#include <sstream>

namespace Ovis
{
	// Helper function to format a float with a specified precision
	std::string Utils::FormatFloat(float value, int precision)
	{
		std::ostringstream oss;
		float multiplier = std::pow(10.0f, precision);
		float roundedValue = std::round(value * multiplier) / multiplier;
		oss << roundedValue;
		return oss.str();
	}

	// Function to convert glm::mat4 to string
	std::string Utils::Mat4ToString(const glm::mat4& matrix)
	{
		std::string result;
		for (int row = 0; row < 4; ++row)
		{
			for (int col = 0; col < 4; ++col)
			{
				result += FormatFloat(matrix[col][row]); // Format the float
				if (col < 3)
				{
					result += ", "; // Add comma between values
				}
			}
			if (row < 3)
			{
				result += "\n"; // Add newline between rows
			}
		}
		return result;
	}
}