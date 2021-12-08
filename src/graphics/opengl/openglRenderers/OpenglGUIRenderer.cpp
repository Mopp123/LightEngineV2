
#include <glew.h>
#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"
#include "entities\Transform.h"
#include "entities\Entity.h"
#include "OpenglGUIRenderer.h"
#include "core\Application.h"


namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{

			static bool batch_identifier_func(gui::GUIImage* img1, gui::GUIImage* img2)
			{
				return img1->getTexture() == img2->getTexture();
			}

			OpenglGUIRenderer::OpenglGUIRenderer()
			{
				_shader = new OpenglGUIImageShader;
			}
			
			OpenglGUIRenderer::~OpenglGUIRenderer()
			{
				delete _shader;
			}

			void OpenglGUIRenderer::submit(const gui::GUIImage* guiImage)
			{
				submitGUIData(guiImage);
			}

			void OpenglGUIRenderer::flush()
			{
				// JUST TESTING
				if (_batches_guiImages.empty())
					return;

				const OpenglMesh* glMesh = (const OpenglMesh*)_mesh_guiImage;

				_shader->bind();
				_shader->setProjectionMatrix(Window::get_projection_matrix()); // *NOTE We don't need to set this each frame, if the window doesn't resize

				for (auto& batch : _batches_guiImages)
				{
					std::vector<float>& batchInstancedData = batch.second;
					
					// Update the "instanced buffer" with the instanced data we got from all submits
					_mesh_guiImage->getVertexBuffers()[1]->update(&batchInstancedData[0], batchInstancedData.size(), 0);
					
					glBindVertexArray(glMesh->getVaoID());

					for (VertexBuffer* vb : glMesh->getVertexBuffers())
					{
						for (VertexBufferElement& vbElement : vb->getLayout().getElements())
						{
							glEnableVertexAttribArray(vbElement.slot);
						}
					}

					glActiveTexture(GL_TEXTURE0);
					
					const Texture* texture = batch.first;
					if (texture)
						glBindTexture(GL_TEXTURE_2D, texture->getID());
					else
						glBindTexture(GL_TEXTURE_2D, _defaultTexture->getID());


					glDrawElementsInstanced(
						GL_TRIANGLE_STRIP,
						glMesh->getIndexBuffer()->getCount(),
						GL_UNSIGNED_INT,
						nullptr,
						batchInstancedData.size()
					);

					glBindVertexArray(0);
					_shader->unbind();

				}

				// reset these before we start submitting on the next frame!
				_batches_guiImages.clear();
				_count_guiImages = 0;
			}
		}
	}
}