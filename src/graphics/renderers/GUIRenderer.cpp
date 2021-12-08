
#include <glew.h>
#include "GUIRenderer.h"
#include "entities\Entity.h"
#include "graphics\Graphics.h"
#include "graphics\opengl\openglRenderers\OpenglGUIRenderer.h"

#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{


		GUIRenderer::GUIRenderer()
		{
			// instanced data goes following way..
			//	pos.x, pos.y, scale.x, scale.y,
			//	color.x,color.y,color.z,color.w
			//	-> 8 floats
			
			
			_batches_guiImages.reserve(_maxGuiImages);

			float vertices[] =
			{
				-1.0f, 1.0f,
				-1.0f, -1.0f,
				1.0f, 1.0f,
				1.0f, -1.0f
			};

			unsigned int indices[] =
			{
				0,1,2,3
			};

			IndexBuffer* ib = IndexBuffer::create(indices, 4, BufferUsage::StaticDraw);
			VertexBuffer* vb_vertexPositions = VertexBuffer::create(vertices, 8, BufferUsage::StaticDraw);
			VertexBuffer* vb_instancedData = VertexBuffer::create(sizeof(float) * 8 * _maxGuiImages, BufferUsage::DynamicDraw);

			VertexBufferLayout layout_vertexPositions(
				{
					{ ShaderDataType::Float2, "position"}
				}
			);

			VertexBufferLayout layout_instancedData(
				{
					{ ShaderDataType::Float4, "transform", 1 },
					{ ShaderDataType::Float4, "color", 1 }
				}
			);

			vb_vertexPositions->setLayout(layout_vertexPositions);
			vb_instancedData->setLayout(layout_instancedData);

			_mesh_guiImage = Mesh::create(ib, { vb_vertexPositions, vb_instancedData }, PrimitiveDrawType::TriangleStrip);

			// Create default texture for all images that doesn't have any texture specified (flat color only)
			unsigned char* pixels = new unsigned char[3];
			memset(pixels, 255, 3);
			_defaultTexImgData = new ImageData(pixels, 1, 1, 3);
			_defaultTexture = Texture::create(_defaultTexImgData, TextureFilteringType::Nearest, TextureImageMapping::Repeat);
		}

		GUIRenderer::~GUIRenderer()
		{
			delete _defaultTexture;
			delete _defaultTexImgData;
			delete _mesh_guiImage;
		}

		GUIRenderer* GUIRenderer::create()
		{
			switch (Graphics::get_graphics_API())
			{
			case GraphicsAPI::OpenGL: return new opengl::OpenglGUIRenderer;

			default:
				break;
			}

			Debug::log("ERROR: Tried to create GUIRenderer, but invalid graphics API was in use!");
			return nullptr;
		}

		// Updates the mesh's "instanced" vertex buffer that holds every gui image's transform data 
		// (Doesn't care about rendering api!)
		void GUIRenderer::submitGUIData(const gui::GUIImage* guiImage)
		{
#ifdef LIGHT_ENGINE__DEBUG_MODE__FULL
			if (!guiImage)
			{
				Debug::log(
					"ERROR: Location: GUIRenderer::submitGUIData(const gui::GUIImage*)\n"
					"Tried to submit GUIImage but the GUIImage was nullptr!"
				);
				return;
			}
			if (_count_guiImages + 1 > _maxGuiImages)
			{
				Debug::log(
					"ERROR: Location: GUIRenderer::submitGUIData(const gui::GUIImage*)\n"
					"Maximum GUI image count reached!\n"
					"Currently maximum GUI image count is " + std::to_string(_maxGuiImages)
				);
				return;
			}
#endif
			insertToBatch(guiImage);
		}


		// Tries to find already existing batch for inputted guiImage depending on its' texture.
		// If batch was found -> adds this image to that batch.
		// If batch wasn't found -> creates new batch for images that uses the same texture.
		void GUIRenderer::insertToBatch(const gui::GUIImage* guiImage)
		{
			for (auto& batch : _batches_guiImages)
			{
				// If a batch is full -> go create another
				if (batch.second.size() >= _maxBatchSize)
					continue;

				if (batch.first == guiImage->getTexture())
				{
					storeInstancedData(guiImage, batch.second);
					return;
				}
			}
			createNewBatch(guiImage);
		}

		// Creates new batch for guiImage
		void GUIRenderer::createNewBatch(const gui::GUIImage* guiImage)
		{
			std::vector<float> instancedData;
			instancedData.reserve(_maxBatchSize);
			storeInstancedData(guiImage, instancedData);
			
			_batches_guiImages.push_back(std::make_pair(guiImage->getTexture(), instancedData));
		}


		// Gets GUIImage's instanced data
		void GUIRenderer::storeInstancedData(const gui::GUIImage* guiImage, std::vector<float>& storage)
		{
			const Transform& imgTransform = guiImage->getEntity()->getTransform();
			const mml::Vec3& pos = imgTransform.getGlobalPosition();
			const mml::Vec3& scale = imgTransform.getGlobalScale();
			const mml::Vec4& color = guiImage->getColor();

			storage.emplace_back(pos.x);
			storage.emplace_back(pos.y);
			storage.emplace_back(scale.x);
			storage.emplace_back(scale.y);
		
			storage.emplace_back(color.x);
			storage.emplace_back(color.y);
			storage.emplace_back(color.z);
			storage.emplace_back(color.w);
		}
	}
}
