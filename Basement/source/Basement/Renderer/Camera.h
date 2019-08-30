#pragma once

#include <glm/glm.hpp>

namespace Basement {

	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

		inline void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }


		virtual const glm::mat4& GetProjectionMatrix() const = 0;
		virtual const glm::mat4& GetViewMatrix() const = 0;
		virtual void RecalculateViewMatrix() = 0;

	protected:
		//glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Position;
	};


	class PerpectiveCamera : public Camera
	{
	public:
		PerpectiveCamera();
		~PerpectiveCamera() = default;

		inline void SetRotation(const glm::mat4& rotation) { m_RotationMatrix = rotation; }

		inline const glm::mat4& GetRotationMatrix() const { return m_RotationMatrix; }
		virtual const glm::mat4& GetViewMatrix() const override { return m_RotationMatrix; }
		virtual const glm::mat4& GetProjectionMatrix() const override { return m_RotationMatrix; }

		virtual void RecalculateViewMatrix() override;	// TODO: to be declared

	private:
		glm::mat4 m_PerspectiveMatrix;
		glm::mat4 m_RotationMatrix;
	};


	class OrthographicCamera : public Camera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);
		~OrthographicCamera() = default;

		inline void SetRotation(const float rotation) { m_Rotation = rotation; }
		inline const float& GetRotation() const { return m_Rotation; }
		virtual const glm::mat4& GetViewMatrix() const override { return m_ViewMatrix; }
		virtual const glm::mat4& GetProjectionMatrix() const override { return m_OrthographicMatrix; }

		virtual void RecalculateViewMatrix() override;	// TODO: to be declared

	private:
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_OrthographicMatrix;
		float m_Rotation = 0.0f;
	};

}