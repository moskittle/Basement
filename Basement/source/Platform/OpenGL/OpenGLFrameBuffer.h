#pragma once

#include "Basement/Renderer/FrameBuffer.h"

namespace Basement {

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(u32 width, u32 height);
		~OpenGLFrameBuffer() = default;

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void BindTexture(u32 slot = 0) const override;

		virtual u32 GetFrameBufferID() const override { return m_FrameBufferID; }
		virtual u32 GetColorAttachmentID() const override { return m_ColorAttachmentID; }
		virtual u32 GetDepthAttachmentID() const override { return m_DepthAttachmentID; }
	private:
		void CreateColorAttachment(u32 width, u32 height);
		void CreateDepthAttechment(u32 width, u32 height);
	private:
		u32 m_FrameBufferID;
		u32 m_ColorAttachmentID;
		u32 m_DepthAttachmentID;

	};




}