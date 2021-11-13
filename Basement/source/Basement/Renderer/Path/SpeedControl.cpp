#include "bmpch.h"
#include "SpeedControl.h"

namespace Basement
{

	void SpeedControl::SetEaseInInterval(float easeInStart, float easeInEnd)
	{
		m_EaseInStart = easeInStart;
		m_EaseInEnd = easeInEnd;
	}

	void SpeedControl::SetEaseOutInterval(float easeOutStart, float easeOutEnd)
	{
		m_EaseOutStart = easeOutStart;
		m_EaseOutEnd = easeOutEnd;
	}



	/// <summary>
	/// This function calculates the max speed of the character on the path, which is the height of the trapezoid
	/// (topLength + bottomLength) * height / 2 = area
	///  |   -----------
	///  |  /           \
	///  | /             \
	///  |/               \
	///  -------------------
	/// </summary>
	void SpeedControl::CalculateMaxSpeed()
	{
		float bottomLength = m_EaseOutEnd - m_EaseInStart;
		float topLength = m_EaseOutStart - m_EaseInEnd;

		m_MaxSpeed = 2.0f / (topLength + bottomLength);
	}

	/// <summary>
	/// Calculate the linear speed the character moves while moving along the path
	/// </summary>
	/// <param name="elapsingTime"></param>
	/// <returns></returns>
	float SpeedControl::CalculateCurrentSpeed(float elapsingTime)
	{
		// accelerating
		if (elapsingTime >= m_EaseInStart && elapsingTime <= m_EaseInEnd)
		{
			return m_MaxSpeed * elapsingTime / m_EaseInEnd;
		}
		// constant speed
		else if (elapsingTime > m_EaseInEnd && elapsingTime < m_EaseOutStart)
		{
			return m_MaxSpeed;
		}
		// decelerating
		else if (elapsingTime >= m_EaseOutStart && elapsingTime <= m_EaseOutEnd)
		{
			return m_MaxSpeed * (m_EaseOutEnd - elapsingTime) / (m_EaseOutEnd - m_EaseOutStart);
		}

		return 0.0f;
	}

	/// <summary>
	/// Calculate the angular speed the character rotates while moving along the path
	/// </summary>
	/// <param name="elapsingTime"></param>
	/// <returns></returns>
	float SpeedControl::CalculateCurrentPosition(float elapsingTime)
	{
		if (elapsingTime >= m_EaseInStart && elapsingTime <= m_EaseInEnd)
		{
			return CalculateCurrentSpeed(elapsingTime) * elapsingTime / 2.0f;
		}
		else if (elapsingTime > m_EaseInEnd && elapsingTime < m_EaseOutStart)
		{
			return (m_MaxSpeed * m_EaseInEnd / 2.0f) + (m_MaxSpeed * (elapsingTime - m_EaseInEnd));
		}
		else if (elapsingTime >= m_EaseOutStart && elapsingTime <= m_EaseOutEnd)
		{
			return 1.0f - (CalculateCurrentSpeed(elapsingTime) * (m_EaseOutEnd - elapsingTime) / 2.0f);
		}

		return 0.0f;
	}

}

