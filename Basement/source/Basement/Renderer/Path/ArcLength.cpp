#include "bmpch.h"

#include "ArcLength.h"


namespace Basement
{

	ArcLength::ArcLength(const std::vector<glm::vec3>& points)
	{
		m_CurvePointPositions = points;

		SetupLookUpTable();
	}

	void ArcLength::SetupLookUpTable()
	{
		m_CurveSegments.clear();
		m_CurveSegments.push_back(TableEntry{ 0.0f, 1.0f, 0.0f });

		m_ArcLengthList.clear();
		m_ArcLengthList.push_back(TableEntry{ 0.0f, 0.0f, 0.0f });

		while (!m_CurveSegments.empty())
		{
			TableEntry entry = m_CurveSegments.front();
			m_CurveSegments.pop_front();

			// Compute the midpoint of the curve at um=(ua+ub)/2.
			auto& startValue = entry.StartValue;
			auto& endValue = entry.EndValue;

			float midValue = (startValue + startValue) / 2.0f;
			glm::vec3 u_a = GetPositionOnCurve(startValue);
			glm::vec3 u_b = GetPositionOnCurve(endValue);
			glm::vec3 u_m = GetPositionOnCurve(midValue);

			// Lengths of two halves and that of entire segment are estimated by linear distances.
			float A = glm::length(u_m - u_a);
			float B = glm::length(u_b - u_m);
			float C = glm::length(u_b - u_a);
			float d = A + B - C;

			// If d is above a user defined error threshold, both halves are added into the segment list to be test
			if (d > m_ErrorThreshold && (endValue - startValue) > m_MaxInterval)
			{
				//m_CurveSegments.push_front(TableEntry{ midValue, endValue, entry.Length });
				//m_CurveSegments.push_front(TableEntry{ startValue, midValue, entry.Length });
				m_CurveSegments.push_back(TableEntry{ startValue, midValue, entry.Length });
				m_CurveSegments.push_back(TableEntry{ midValue, endValue, entry.Length });
			}
			else
			{
				// Otherwise, record  two new segments
				float& length = m_ArcLengthList.back().Length;
				m_ArcLengthList.push_back(TableEntry{ startValue, midValue, length + A });
				m_ArcLengthList.push_back(TableEntry{ midValue, endValue, length + A + B });
			}
		}

		NormalizeLookUpTable();
	}

	/// <summary>
	/// This function uses binary search to get value to look up
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	float ArcLength::GetValueFromLookUpTable(float value)
	{
		u32 start = 0;
		u32 end = static_cast<u32>(m_ArcLengthList.size()) - 1;

		// Binary search
		while (end - start > 1)
		{
			int mid = (start + end) / 2;
			if (value >= m_ArcLengthList[mid].Length)
			{
				start = mid;
			}
			else
			{
				end = mid;
			}
		}

		// Equation: u = ui + du * (s - si) / (si+1 - si)
		float du = m_ArcLengthList[end].EndValue - m_ArcLengthList[end].StartValue;
		float startLength = m_ArcLengthList[start].Length;
		float endLength = m_ArcLengthList[end].Length;

		float ui = m_ArcLengthList[end].StartValue;
		float result = ui + du * (value - startLength) / (endLength - startLength);

		return result;
	}

	/// <summary>
	/// Get the acutally point position on path
	/// </summary>
	/// <param name="value"></param>
	/// <returns></returns>
	glm::vec3 ArcLength::GetPositionOnCurve(float value)
	{
		int pointCount = static_cast<int>(m_CurvePointPositions.size());
		int index = static_cast<int>(value * (pointCount - 1));
		index = glm::clamp(index, 0, pointCount - 1);

		return m_CurvePointPositions[index];
	}

	ArcLength& ArcLength::operator=(const ArcLength& rhs)
	{
		// TODO: insert return statement here
		this->m_CurvePointPositions = rhs.m_CurvePointPositions;
		this->m_ArcLengthList = rhs.m_ArcLengthList;
		this->m_CurveSegments = rhs.m_CurveSegments;
		this->m_ErrorThreshold = rhs.m_ErrorThreshold;
		this->m_MaxInterval = rhs.m_MaxInterval;

		return *this;
	}


	/// <summary>
	/// This funtion normalizes the length of entries in arc length list
	/// </summary>
	void ArcLength::NormalizeLookUpTable()
	{
		float& totalLength = m_ArcLengthList.back().Length;
		for (auto& entry : m_ArcLengthList)
		{
			entry.Length /= totalLength;
		}
	}

}