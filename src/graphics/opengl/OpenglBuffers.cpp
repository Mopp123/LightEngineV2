
#include <glew.h>
#include "OpenglBuffers.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{

			static GLenum convert_buffer_usage_to_opengl(BufferUsage usage)
			{
				switch (usage)
				{
					case BufferUsage::StaticDraw:	return GL_STATIC_DRAW;
					case BufferUsage::DynamicDraw:	return GL_DYNAMIC_DRAW;
					case BufferUsage::StreamDraw:	return GL_STREAM_DRAW;

					default:
						break;
				}

				Debug::log("ERROR: Location: convert_buffer_usage_to_opengl_usage(BufferUsage)! Inputted invalid BufferUsage!");
				return GL_NONE;
			}

			OpenglVertexBuffer::OpenglVertexBuffer(size_t dataSize, BufferUsage usage, bool saveRawData) :
				VertexBuffer(dataSize, saveRawData)
			{
				glCreateBuffers(1, &_id);
				glBindBuffer(GL_ARRAY_BUFFER, _id);
				glBufferData(GL_ARRAY_BUFFER, dataSize, nullptr, convert_buffer_usage_to_opengl(usage));
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			OpenglVertexBuffer::OpenglVertexBuffer(float* data, unsigned int dataLength, BufferUsage usage, bool saveRawData) :
				VertexBuffer(data, dataLength, saveRawData)
			{
				glCreateBuffers(1, &_id);
				glBindBuffer(GL_ARRAY_BUFFER, _id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dataLength, data, convert_buffer_usage_to_opengl(usage));
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			OpenglVertexBuffer::OpenglVertexBuffer(int* data, unsigned int dataLength, BufferUsage usage, bool saveRawData) :
				VertexBuffer(data, dataLength, saveRawData)
			{
				glCreateBuffers(1, &_id);
				glBindBuffer(GL_ARRAY_BUFFER, _id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(int) * dataLength, data, convert_buffer_usage_to_opengl(usage));
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}


			OpenglVertexBuffer::~OpenglVertexBuffer()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glDeleteBuffers(1, &_id);
			}

			void OpenglVertexBuffer::bind()
			{
				glBindBuffer(GL_ARRAY_BUFFER, _id);
			}

			void OpenglVertexBuffer::unbind()
			{
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}


			// Replaces the data from the buffer.. starting from the offset
			void OpenglVertexBuffer::update(float* data, unsigned int dataLength, int offset)
			{
				if (_saveRawData)
				{
					// This buffer owns its' data 
					//	-> so delete the current data
					delete[] _data;
					// exchange the ownership of the inputted data
					_data = data;
				}

				glCall(glBindBuffer(GL_ARRAY_BUFFER, _id));
				glCall(glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(float) * dataLength, data));
				glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
			}


			OpenglIndexBuffer::OpenglIndexBuffer(unsigned int* indices, unsigned int count, BufferUsage usage, bool saveRawData) :
				IndexBuffer(indices, count, saveRawData)
			{
				glCreateBuffers(1, &_id);

				// Thanks to "Thecherno" I was able to get around this vao binding problem with opengl index buffers..

				// *Copied from Hazel project:
				//		"GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
				//		Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state."
				glBindBuffer(GL_ARRAY_BUFFER, _id);
				glBufferData(GL_ARRAY_BUFFER, sizeof(unsigned int) * count, indices, convert_buffer_usage_to_opengl(usage));

				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			OpenglIndexBuffer::~OpenglIndexBuffer()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glDeleteBuffers(1, &_id);
			}

			void OpenglIndexBuffer::bind()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
			}

			void OpenglIndexBuffer::unbind()
			{
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		}
	}
}