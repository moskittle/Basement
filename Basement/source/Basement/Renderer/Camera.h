#pragma once

#include <glm/glm.hpp>

namespace Basement {

	class Camera2D
	{
	public:
		Camera2D(float left, float right, float bottom, float top);
		~Camera2D() = default;

		void SetProjectionMatrix(float left, float right, float bottom, float top);
		
		void SetPosition(const glm::vec3& position);
		void SetRotation(float rotation);

		inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		inline const float& GetRotation() const { return m_Rotation; }

		void UpdateViewProjectionMatrix();
	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position;
		float m_Rotation;
	};

}