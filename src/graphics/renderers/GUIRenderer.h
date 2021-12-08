#pragma once

#include "graphics\Renderer.h"
#include "graphics\graphicsResources\Mesh.h"
#include "GUI\GUIImage.h"

namespace lightEngine
{
	namespace graphics
	{

		struct InstancedImageData
		{
			float posX;
			float posY;
			float width;
			float height;

			float color_r;
			float color_g;
			float color_b; 
			float color_a;
		};

		class GUIRenderer
		{
		protected:

			// Every gui image uses the same mesh
			Mesh* _mesh_guiImage;

			ImageData* _defaultTexImgData = nullptr;
			Texture* _defaultTexture = nullptr;

			std::vector<std::pair<const Texture*, std::vector<float>>> _batches_guiImages;

			const unsigned int _maxGuiImages = 1000000;
			const unsigned int _maxBatchSize = 100000;
			// Amount of submitted guiImages
			unsigned int _count_guiImages = 0;

		public:
			
			GUIRenderer();
			virtual ~GUIRenderer();

			virtual void submit(const gui::GUIImage* guiImage) = 0;
			virtual void flush() = 0;

			static GUIRenderer* create();

		protected:

			// Updates the mesh's "instanced" vertex buffer that holds every gui image's transform data 
			// (Doesn't care about rendering api!)
			void submitGUIData(const gui::GUIImage* guiImage);

			// Tries to find already existing batch for inputted guiImage depending on its' texture.
			// If batch was found -> adds this image to that batch.
			// If batch wasn't found -> creates new batch for images that uses the same texture.
			void insertToBatch(const gui::GUIImage* guiImage);

			// Creates new batch for guiImage
			void createNewBatch(const gui::GUIImage* guiImage);

			// Gets GUIImage's instanced data
			void storeInstancedData(const gui::GUIImage* guiImage, std::vector<float>& storage);
		};
	}
}