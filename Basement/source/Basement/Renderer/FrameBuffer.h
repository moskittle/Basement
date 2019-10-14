#pragma once


namespace Basement {

	enum class FrameBufferFormat
	{
		None = 0,
		RGBA8 = 1,
		RGBA16F = 2
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		static SharedPtr<FrameBuffer> Create(u32 width, u32 height);

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void ActivateTexture(u32 slot = 0) const = 0;

		virtual u32 GetFrameBufferID() const = 0;
		virtual u32 GetColorAttachmentID() const = 0;
		virtual u32 GetDepthAttachmentID() const = 0;
	private:

	};

	class FrameBufferPool
	{
	public:

	private:


	};



}