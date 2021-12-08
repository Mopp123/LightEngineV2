

#include "Buffers.h"
#include "opengl\OpenglBuffers.h"

#include "graphics\Graphics.h"

#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		size_t get_shader_data_type__size(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Int:		return sizeof(int);
			case ShaderDataType::Int2:		return sizeof(int) * 2;
			case ShaderDataType::Int3:		return sizeof(int) * 3;
			case ShaderDataType::Int4:		return sizeof(int) * 4;

			case ShaderDataType::Float:		return sizeof(float);
			case ShaderDataType::Float2:	return sizeof(float) * 2;
			case ShaderDataType::Float3:	return sizeof(float) * 3;
			case ShaderDataType::Float4:	return sizeof(float) * 4;

			case ShaderDataType::Mat4:		return sizeof(float) * 16;

			default:
				break;
			}

			Debug::log("ERROR: Location: get_shader_data_type__size(ShaderDataType). Inputted ShaderDataType was invalid!");
			return 0;
		}

		int get_shader_data_type__elem_count(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;

			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;

			case ShaderDataType::Mat4:		return 16;

			default:
				break;
			}

			Debug::log("ERROR: Location: get_shader_data_type__elem_count(ShaderDataType). Inputted ShaderDataType was invalid!");
			return 0;
		}

		bool shader_data_type__is_matrix(ShaderDataType type)
		{
			switch (type)
			{
			case ShaderDataType::Mat4:		return true;
			default:
				break;
			}

			return false;
		}

		VertexBufferLayout::VertexBufferLayout(const std::vector<VertexBufferElement>& elements) :
			_elements(elements)
		{
			// * NOTE we cannot calc slot here, since we don't have enough knowledge of the other vertex buffers here..
			size_t prevSize = 0;

			for (VertexBufferElement& e : _elements)
			{
				e.offset += _stride;

				size_t elemSize = get_shader_data_type__size(e.shaderDataType);
				_stride += (int)elemSize;
				prevSize = elemSize;
			}
		}

		// This creates "empty" vertex buffer. You still need to give its' size in bytes!
		VertexBuffer::VertexBuffer(size_t dataSize, bool saveRawData) :
			_saveRawData(saveRawData)
		{
		}
		VertexBuffer::VertexBuffer(float* data, unsigned int dataLength, bool saveRawData) : 
			_length(dataLength), _saveRawData(saveRawData)
		{
			if (_saveRawData)
				_data = data;
		}
		VertexBuffer::VertexBuffer(int* data, unsigned int dataLength, bool saveRawData) :
			_length(dataLength), _saveRawData(saveRawData)
		{
		}

		VertexBuffer::~VertexBuffer()
		{
			if(_data)
				delete[] _data;
		}

		// Creates and returns vertex buffer according to the current graphics api
		VertexBuffer* VertexBuffer::create(unsigned int dataLength, BufferUsage usage, bool saveRawData)
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglVertexBuffer(dataLength, usage, saveRawData);

			default:
				break;
			}

			Debug::log("ERROR: Tried to create vertex buffer, but invalid graphics API was in use!");
			return nullptr;
		}

		VertexBuffer* VertexBuffer::create(float* data, unsigned int dataLength, BufferUsage usage, bool saveRawData)
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglVertexBuffer(data, dataLength, usage, saveRawData);

			default:
				break;
			}

			Debug::log("ERROR: Tried to create vertex buffer, but invalid graphics API was in use!");
			return nullptr;
		}

		VertexBuffer* VertexBuffer::create(int* data, unsigned int dataLength, BufferUsage usage, bool saveRawData)
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglVertexBuffer(data, dataLength, usage, saveRawData);

			default:
				break;
			}

			Debug::log("ERROR: Tried to create vertex buffer, but invalid graphics API was in use!");
			return nullptr;
		}



		IndexBuffer::IndexBuffer(unsigned int* indices, unsigned int count, bool saveRawData) : 
			_count(count), _saveRawData(saveRawData)
		{
			if (saveRawData)
				_indices = indices;
		}

		// Creates and returns index buffer according to the current graphics api
		IndexBuffer* IndexBuffer::create(unsigned int* indices, unsigned int count, BufferUsage usage, bool saveRawData)
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglIndexBuffer(indices, count, usage, saveRawData);

			default:
				break;
			}

			Debug::log("ERROR: Tried to create index buffer, but invalid graphics API was in use!");
			return nullptr;
		}

		IndexBuffer::~IndexBuffer()
		{
			if (_indices)
				delete[] _indices;
		}
	}
}