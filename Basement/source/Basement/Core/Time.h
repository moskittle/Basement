#pragma once

namespace Basement {

	class Timer
	{
	public:
		Timer(float currentFrameTime = 0.0f, float lastFrameTime = 0.0f, float deltaTime = 0.0f)
			: m_CurrentFrameTime(currentFrameTime), m_LastFrameTime(lastFrameTime), m_DeltaTime(deltaTime) {}

		inline float GetDeltaTimeInSeconds() const { return m_DeltaTime; }
		inline float GetDeltaTimeInMilliseconds() const { return m_DeltaTime * 1000.0f; }

		inline void SetCurrentFrameTime(float currentFrameTime) { m_CurrentFrameTime = currentFrameTime; }
		inline void CalculateDeltaTime() { m_DeltaTime = m_CurrentFrameTime - m_LastFrameTime; m_LastFrameTime = m_CurrentFrameTime; }

		operator float() const { return m_DeltaTime; }
	private:
		float m_CurrentFrameTime;
		float m_LastFrameTime;
		float m_DeltaTime;
	};

}