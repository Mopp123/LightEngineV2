#pragma once

#include "graphics\Buffers.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{

			class OpenglVertexBuffer : public VertexBuffer
			{
			public:

				OpenglVertexBuffer(size_t dataSize, BufferUsage usage, bool saveRawData);
				OpenglVertexBuffer(float* data, unsigned int dataLength, BufferUsage usage, bool saveRawData);
				OpenglVertexBuffer(int* data, unsigned int dataLength, BufferUsage usage, bool saveRawData);
				~OpenglVertexBuffer();

				virtual void bind() override;
				virtual void unbind() override;

				// Replaces the data from the buffer.. starting from the offset
				virtual void update(float* data, unsigned int dataLength, int offset) override;
			};


			class OpenglIndexBuffer : public IndexBuffer
			{
			public:

				OpenglIndexBuffer(unsigned int* indices, unsigned int count, BufferUsage usage, bool saveRawData);
				~OpenglIndexBuffer();

				virtual void bind() override;
				virtual void unbind() override;
			};
		}
	}
}