
#include <glew.h>
#include "OpenglMesh.h"

#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			static GLenum convert_shader_data_type_to_opengl_type(ShaderDataType type)
			{
				switch (type)
				{
				case ShaderDataType::Int:		return GL_INT;
				case ShaderDataType::Int2:		return GL_INT;
				case ShaderDataType::Int3:		return GL_INT;
				case ShaderDataType::Int4:		return GL_INT;

				case ShaderDataType::Float:		return GL_FLOAT;
				case ShaderDataType::Float2:	return GL_FLOAT;
				case ShaderDataType::Float3:	return GL_FLOAT;
				case ShaderDataType::Float4:	return GL_FLOAT;

				case ShaderDataType::Mat4:		return GL_FLOAT;

				default:
					break;
				}

				Debug::log("ERROR: Location: convert_shader_data_type_to_opengl_type(ShaderDataType). Inputted invalid shader data type!");
				return GL_NONE;
			}



			// *NOTE Mat4 type doesn't work as attribute!!!
			OpenglMesh::OpenglMesh(IndexBuffer* indexBuffer, std::vector<VertexBuffer*> vertexBuffers, PrimitiveDrawType drawType) :
				Mesh(drawType)
			{
				_indexBuffer = indexBuffer;
				_vertexBuffers = vertexBuffers;

				glCall(glCreateVertexArrays(1, &_vaoID));
				glCall(glBindVertexArray(_vaoID));

				glCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBuffer->getID()));

				unsigned int slot = 0;

				for (VertexBuffer* vb : vertexBuffers)
				{
					glCall(glBindBuffer(GL_ARRAY_BUFFER, vb->getID()));
					for (VertexBufferElement& vbElement : vb->getLayout().getElements())
					{

						const ShaderDataType& dataType = vbElement.shaderDataType;
						int dataTypeElemCount = get_shader_data_type__elem_count(dataType);

						// Always increment the slot when calling glVertexAttribPointer()..
						//vbElement.slot = slot++;
						vbElement.slot = slot;

						// if this element is matrix, we have more than 1 occupied "slots"
						int elemSlotCount = 1; // count of 1 means this doesn't have any "sub elements"
						if (shader_data_type__is_matrix(dataType))
						{
							elemSlotCount = (int)std::sqrt(dataTypeElemCount);
							dataTypeElemCount = elemSlotCount;
						}

						for (int i = 0; i < elemSlotCount; i++)
						{
							if (i > 0)
								slot++;

							glCall(glEnableVertexAttribArray(slot));

							GLenum glDataType = convert_shader_data_type_to_opengl_type(dataType);

							if (glDataType == GL_FLOAT)
							{
								glCall(glVertexAttribPointer(
									slot,
									dataTypeElemCount,
									glDataType,
									GL_FALSE,
									vb->getLayout().getStride(),
									(const void*)vbElement.offset
								));
							}
							else if (glDataType == GL_INT)
							{
								glCall(glVertexAttribIPointer(
									slot,
									dataTypeElemCount,
									glDataType,
									vb->getLayout().getStride(),
									(const void*)vbElement.offset
								));
							}

							glCall(glVertexAttribDivisor(vbElement.slot, vbElement.divisor));

							glCall(glDisableVertexAttribArray(vbElement.slot));
						}

						slot++;
					}
				}

				glCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
				glCall(glBindVertexArray(0));
			}


			OpenglMesh::~OpenglMesh()
			{
				glBindVertexArray(0);
				glDeleteVertexArrays(1, &_vaoID);
			}
		}
	}
}