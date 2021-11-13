#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <deque>

namespace Basement
{

	struct TableEntry
	{
		float StartValue;
		float EndValue;
		float Length;
	};

	class ArcLength
	{
	public:
		ArcLength() = default;
		ArcLength(const std::vector<glm::vec3>& points);
		~ArcLength() = default;

		void SetupLookUpTable();
		float GetValueFromLookUpTable(float value);
		int GetPositionOnCurveIndex(float value);
		glm::vec3 GetPositionOnCurve(int index);

		ArcLength& operator=(const ArcLength& rhs);
	private:
		void NormalizeLookUpTable();

		std::vector<glm::vec3> m_CurvePointPositions;
		std::vector<TableEntry> m_ArcLengthList;
		std::deque<TableEntry> m_CurveSegments;

		float m_ErrorThreshold = 0.01f;
		float m_MaxInterval = 0.01f;
	};

}