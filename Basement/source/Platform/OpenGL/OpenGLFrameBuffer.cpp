#include "bmpch.h"

#include "OpenGLFrameBuffer.h"
#include "OpenGLDebug.h"

#include <glad/glad.h>

namespace Basement {

	OpenGLFrameBuffer::OpenGLFrameBuffer(u32 width, u32 height)
	{
		// Create frambuffer
		OpenGLCall(glGenFramebuffers(1, &m_FrameBufferID));
		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));

		// Create attachments
		CreateColorAttachment(width, height);
		CreateDepthAttechment(width, height);

		// Check framebuffer status
		BM_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer incomplete!");
		
		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	}

	void OpenGLFrameBuffer::CreateColorAttachment(u32 width, u32 height)
	{
		OpenGLCall(glGenTextures(1, &m_ColorAttachmentID));
		OpenGLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID));

		OpenGLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr));
		OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		OpenGLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		OpenGLCall(glBindTexture(GL_TEXTURE_2D, 0));

		OpenGLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0));
	}

	void OpenGLFrameBuffer::CreateDepthAttechment(u32 width, u32 height)
	{
		OpenGLCall(glGenRenderbuffers(1, &m_DepthAttachmentID));
		OpenGLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID));
		OpenGLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
		OpenGLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

		OpenGLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachmentID));
	}

	void OpenGLFrameBuffer::Bind() const
	{
		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID));
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		OpenGLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

	void OpenGLFrameBuffer::ActivateTexture(u32 slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		OpenGLCall(glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID));
	}

}