#pragma once

#include "../Buffer.h"
#include "../Shader.h"
#include "../VertexArray.h"

#include <vector>

#include <glm/glm.hpp>

namespace Basement
{

	class Path
	{
	public:
		Path() = default;
		Path(const std::vector<glm::vec3>& controlPoints);
		~Path() = default;

		void Draw(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawPath, bool drawControlPoints);
		void CalculateCurve();
		glm::vec3 CalculatePointPostion(float t);
		const std::vector<glm::vec3>& GetPointsOnCurve() { return m_PointsOnCurve; }
	private:
		void Init();

		int m_PointCount = 100;
		std::vector<glm::vec3> m_ControlPointPositions;
		std::vector<glm::vec3> m_PointsOnCurve;

		std::vector<Vertex> m_PointVertices;
		std::vector<u32> m_PointIndices;
		SharedPtr<VertexArray> m_PathPointVao;
		SharedPtr<VertexBuffer> m_PathPointVbo;
		SharedPtr<IndexBuffer> m_PathPointIbo;

		std::vector<Vertex> m_LineVertices;
		std::vector<u32> m_LineIndices;
		SharedPtr<VertexArray> m_PathLineVao;
		SharedPtr<VertexBuffer> m_PathLineVbo;
		SharedPtr<IndexBuffer> m_PathLineIbo;

	};
}