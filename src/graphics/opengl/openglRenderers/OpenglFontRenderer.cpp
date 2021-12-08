
#include <glew.h>
#include "OpenglFontRenderer.h"
#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"
#include "core\Application.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			OpenglFontRenderer::OpenglFontRenderer()
			{
				_shader = new OpenglFontShader;
			}

			OpenglFontRenderer::~OpenglFontRenderer()
			{}

			void OpenglFontRenderer::submit(const gui::Text* text)
			{
				submitTextData(text);
			}
			
			void OpenglFontRenderer::flush()
			{
				if (_count_characters == 0)
					return;

				glEnable(GL_DEPTH_TEST);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				const OpenglMesh* glMesh = (const OpenglMesh*)_mesh_font;

				// Update the "instanced buffer" with the instanced data we got from all submits
				_mesh_font->getVertexBuffers()[1]->update(
					_instancedBuffer_data,
					_count_characters * FONT_RENDERER__INSTANCED_DATA_ENTRY_LENGTH/*_instancedBuffer_data.size()*/,
					0
				);

				_shader->bind();

				_shader->setProjectionMatrix(Window::get_projection_matrix()); // *NOTE We don't need to set this each frame, if the window doesn't resize
				_shader->setTextureAtlasRowCount(_fontAtlasRowCount); // We actually don't need to set this more than once!!!
				
				glBindVertexArray(glMesh->getVaoID());

				for (VertexBuffer* vb : glMesh->getVertexBuffers())
					for (VertexBufferElement& vbElement : vb->getLayout().getElements())
						glEnableVertexAttribArray(vbElement.slot);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, _fontTexture->getID());

				glDrawElementsInstanced(
					GL_TRIANGLE_STRIP,
					glMesh->getIndexBuffer()->getCount(),
					GL_UNSIGNED_INT,
					nullptr,
					_count_characters
				);

				glBindVertexArray(0);
				_shader->unbind();

				//_instancedBuffer_data.clear();
				_instancedBuffer_pointer = 0;
				_count_characters = 0;

				glEnable(GL_DEPTH_TEST);
				glDisable(GL_BLEND);
			}
		}
	}
}