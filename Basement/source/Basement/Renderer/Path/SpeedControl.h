#pragma once


namespace Basement
{

	class SpeedControl
	{
	public:
		SpeedControl() = default;
		~SpeedControl() = default;

		void SetEaseInInterval(float easeInStart, float easeInEnd);
		void SetEaseOutInterval(float easeOutStart, float easeOutEnd);
		void CalculateMaxSpeed();
		const float GetMaxSpeed() { return m_MaxSpeed; }
		float CalculateCurrentPosition(float dt);
		float CalculateCurrentSpeed(float dt);

	private:
		float m_EaseInStart;
		float m_EaseInEnd;
		float m_EaseOutStart;
		float m_EaseOutEnd;

		float m_MaxSpeed;
	};



}