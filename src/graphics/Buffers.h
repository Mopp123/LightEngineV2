
#pragma once

#include <string>
#include <vector>

namespace lightEngine
{
	namespace graphics
	{

		enum class ShaderDataType
		{
			None = 0,

			Int,
			Int2,
			Int3,
			Int4,

			Float,
			Float2,
			Float3,
			Float4,

			Mat4
		};

		size_t get_shader_data_type__size(ShaderDataType type);
		int get_shader_data_type__elem_count(ShaderDataType type);
		bool shader_data_type__is_matrix(ShaderDataType type);


		enum class BufferUsage
		{
			None = 0, StaticDraw, DynamicDraw, StreamDraw
		};


		struct VertexBufferElement
		{
			unsigned int slot = 0;
			ShaderDataType shaderDataType = ShaderDataType::None;
			std::string name;
			
			int offset = 0;

			unsigned int divisor = 0;

			VertexBufferElement(ShaderDataType dataType, const std::string& name, unsigned int divisor = 0) :
				shaderDataType(dataType), name(name), divisor(divisor)
			{}
		};


		class VertexBufferLayout
		{
		protected:

			std::vector<VertexBufferElement> _elements;
			int _stride = 0;

		public:

			VertexBufferLayout() {}

			// This also calculates each element's stride and offset
			VertexBufferLayout(const std::vector<VertexBufferElement>& elements);
			virtual ~VertexBufferLayout() {}

			inline std::vector<VertexBufferElement>& getElements() { return _elements; }
			inline const int getStride() const { return _stride; }
		};


		class VertexBuffer
		{
		protected:

			float* _data = nullptr;

			unsigned int _length; // length of data array
			size_t _size; // size in bytes
			unsigned int _id;

			VertexBufferLayout _layout;

			// If this is true we always store the "pure raw data" of this buffer on the cpu side as well!
			bool _saveRawData = false;

		public:

			// This creates "empty" vertex buffer. You still need to give its' size in bytes!
			VertexBuffer(size_t dataSize, bool saveRawData = false);
			VertexBuffer(float* data, unsigned int dataLength, bool saveRawData = false);
			// *NOTE This cannot currently save the raw data on cpu side!
			VertexBuffer(int* data, unsigned int dataLength, bool saveRawData = false);

			virtual ~VertexBuffer();

			// Creates and returns vertex buffer according to the current graphics api
			static VertexBuffer* create(unsigned int dataLength, BufferUsage usage, bool saveRawData = false);
			static VertexBuffer* create(float* data, unsigned int dataLength, BufferUsage usage, bool saveRawData = false);
			static VertexBuffer* create(int* data, unsigned int dataLength, BufferUsage usage, bool saveRawData = false);

			virtual void bind() {}
			virtual void unbind() {}

			// Replaces the data from the buffer.. starting from the offset
			virtual void update(float* data, unsigned int dataLength, int offset) {}

			inline void setLayout(const VertexBufferLayout& layout) { _layout = layout; }

			inline float* getData() { return _data; }
			inline const unsigned int getLength() const { return _length; }
			inline const size_t getSize() const { return _size; }
			inline const unsigned int getID() const { return _id; }
			inline VertexBufferLayout& getLayout() { return _layout; }
		};


		class IndexBuffer
		{
		protected:

			unsigned int* _indices = nullptr;
			unsigned int _count; // length of data array
			unsigned int _id;

			bool _saveRawData = false;

		public:

			IndexBuffer(bool saveRawData = false) : _saveRawData(saveRawData) {}
			IndexBuffer(unsigned int* indices, unsigned int count, bool saveRawData = false);
			virtual ~IndexBuffer();

			// Creates and returns index buffer according to the current graphics api
			// *NOTE if using OpenGL, vertex array has to be bound at the time, this function gets called!
			static IndexBuffer* create(unsigned int* indices, unsigned int count, BufferUsage usage, bool saveRawData = false);
			
			virtual void bind() {}
			virtual void unbind() {}

			inline unsigned int* getIndices() { return _indices; }
			inline const unsigned int getCount() const { return _count; }
			inline const unsigned int getID() const { return _id; }
		};
	}
}