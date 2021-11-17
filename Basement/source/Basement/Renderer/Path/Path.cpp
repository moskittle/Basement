#include "bmpch.h"

#include "Path.h"
#include "../Renderer.h"

#include <glm/gtx/compatibility.hpp>

namespace Basement
{

	Path::Path(const std::vector<glm::vec3>& controlPoints) : m_ControlPointPositions(controlPoints)
	{
		Init();
	}

	void Path::Init()
	{
		// Setup points
		for (auto controlPontPos : m_ControlPointPositions)
		{
			auto newPointVertex = Vertex();
			newPointVertex.Position = controlPontPos;
			m_PointVertices.push_back(newPointVertex);
		}

		BufferLayout bufferLayout = {
			{ EShaderDataType::Float3, "a_Position" },
			{ EShaderDataType::Float3, "a_Normal" },
			{ EShaderDataType::Float2, "a_TexCoord" },
			{ EShaderDataType::Int4, "a_BoneIds"},
			{ EShaderDataType::Float4, "a_BoneWeights"}
		};

		m_PathPointVao = VertexArray::Create();
		m_PathPointVbo = VertexBuffer::Create(m_PointVertices);
		m_PathPointVbo->SetLayout(bufferLayout);
		m_PathPointVao->AddVertexBuffer(m_PathPointVbo);

		for (u32 i = 0; i < static_cast<u32>(m_PointVertices.size()); ++i)
		{
			m_PointIndices.push_back(i);
		}

		m_PathPointIbo = IndexBuffer::Create(m_PointIndices);
		m_PathPointVao->SetIndexBuffer(m_PathPointIbo);

		// Setup line
		CalculateCurve();

		m_PathLineVao = VertexArray::Create();
		m_PathLineVbo = VertexBuffer::Create(m_LineVertices);
		m_PathLineVbo->SetLayout(bufferLayout);
		m_PathLineVao->AddVertexBuffer(m_PathLineVbo);
		m_PathLineIbo = IndexBuffer::Create(m_LineIndices);
		m_PathLineVao->SetIndexBuffer(m_PathLineIbo);
	}

	/// <summary>
	/// This functino draws the path and control points
	/// </summary>
	/// <param name="shader">shader used for drawing path and control points</param>
	/// <param name="modelMatrix">transform matrix</param>
	void Path::Draw(SharedPtr<Shader> shader, glm::mat4 modelMatrix, bool drawPath, bool drawControlPoints)
	{
		//draw points
		if (drawControlPoints)
		{
			u32 indexCount = static_cast<u32>(m_PointIndices.size());
			Renderer::SubmitArraysForPoints(shader, m_PathPointVao, m_PointIndices[0], indexCount, modelMatrix);
		}

		// draw lines
		if (drawPath)
		{
			Renderer::SubmitForLines(shader, m_PathLineVao, modelMatrix);
		}
	}

	/// <summary>
	/// This function store the data calculated of the bezier curve.
	/// </summary>
	void Path::CalculateCurve()
	{
		float interval = 1.0f / static_cast<float>(m_PointCount);
		for (u32 i = 0; i < static_cast<u32>(m_PointCount - 1); ++i)
		{
			glm::vec3 newPointPos = CalculatePointPostion(static_cast<float>(i) * interval);
			m_PointsOnCurve.push_back(newPointPos);

			Vertex newVertex = Vertex();
			newVertex.Position = newPointPos;
			m_LineVertices.push_back(newVertex);
			m_LineIndices.push_back(i);
			m_LineIndices.push_back(i + 1);
		}

		// Special case: for the last point, no need to draw a line
		glm::vec3 newPointPos = CalculatePointPostion(1.0f);
		m_PointsOnCurve.push_back(newPointPos);

		Vertex newVertex = Vertex();
		newVertex.Position = newPointPos;
		m_LineVertices.push_back(newVertex);
	}

	/// <summary>
	/// This function uses DeCastlejau Algorithm to calculate a bezier curve with
	/// given control points. 
	/// </summary>
	glm::vec3 Path::CalculatePointPostion(float t)
	{
		glm::vec3 point(0.0f, 0.05f, 0.0f);
		int controlPointCount = static_cast<int>(m_ControlPointPositions.size());

		std::vector<glm::vec3> layer(controlPointCount, point);

		for (u32 i = 0; i < static_cast<u32>(controlPointCount); ++i)
		{
			layer[i].x = m_ControlPointPositions[i].x;
			layer[i].z = m_ControlPointPositions[i].z;
		}

		// The implementation of De Casteljau's algorithm with nested linear interpolation
		for (u32 i = 0; i < static_cast<u32>(controlPointCount) - 1; ++i)
		{
			for (u32 j = 0; j < controlPointCount - 1 - i; ++j)
			{
				layer[j] = glm::lerp(layer[j], layer[j + 1], t);
			}
		}

		point = layer[0];

		return point;
	}

}