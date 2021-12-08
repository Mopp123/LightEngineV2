
#include <glew.h>
#include "core\Debug.h"
#include "OpenglRendererCommands.h"
#include "openglGraphicsResources\OpenglMesh.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			void OpenglRendererCommands::clear()
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			}
			void OpenglRendererCommands::clearColor()
			{
				glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
			}

			void OpenglRendererCommands::bindMesh(const Mesh* mesh) const
			{
				const OpenglMesh* glMesh = (const OpenglMesh*)mesh;
				glCall(glBindVertexArray(glMesh->getVaoID()));

				for (VertexBuffer* vb : glMesh->getVertexBuffers())
				{
					for (VertexBufferElement& vbElem : vb->getLayout().getElements())
					{
						glCall(glEnableVertexAttribArray(vbElem.slot));
						// If this element was a 4x4 matrix -> enable all the rest of the arrays as well.. *DOESN'T WORK AT THE MOMENT!
						if (vbElem.shaderDataType == ShaderDataType::Mat4)
						{
							glCall(glEnableVertexAttribArray(vbElem.slot + 1));
							glCall(glEnableVertexAttribArray(vbElem.slot + 2));
							glCall(glEnableVertexAttribArray(vbElem.slot + 3));
						}
					}
				}
			}

			void OpenglRendererCommands::unbindMesh(const Mesh* mesh) const
			{
				const OpenglMesh* glMesh = (const OpenglMesh*)mesh;
				
				for (VertexBuffer* vb : glMesh->getVertexBuffers())
				{
					for (VertexBufferElement& vbElem : vb->getLayout().getElements())
					{
						glCall(glDisableVertexAttribArray(vbElem.slot));
						// If this element was a 4x4 matrix -> enable all the rest of the arrays as well.. *DOESN'T WORK AT THE MOMENT!
						if (vbElem.shaderDataType == ShaderDataType::Mat4)
						{
							glCall(glDisableVertexAttribArray(vbElem.slot + 1));
							glCall(glDisableVertexAttribArray(vbElem.slot + 2));
							glCall(glDisableVertexAttribArray(vbElem.slot + 3));
						}
					}
				}

				glCall(glBindVertexArray(0));
			}


			void OpenglRendererCommands::bindMaterial(const Material* material) const
			{
				if (material)
				{
					switch (material->getCulledFace())
					{
					case LE__GRAPHICS__MESH_BACK_FACE:
						glEnable(GL_CULL_FACE);
						glCullFace(GL_BACK);
						break;
					case LE__GRAPHICS__MESH_FRONT_FACE:
						glEnable(GL_CULL_FACE);
						glCullFace(GL_FRONT);
						break;
					default:
						glDisable(GL_CULL_FACE);
					}

					const Texture* texture_diffuse = material->getTexture_diffuse();
					const Texture* texture_specular = material->getTexture_specular();
					const Texture* texture_normal = material->getTexture_normal();

					if (texture_diffuse)
					{
						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, texture_diffuse->getID());
					}
					if (texture_specular)
					{
						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, texture_specular->getID());
					}
					if (texture_normal)
					{
						glActiveTexture(GL_TEXTURE2);
						glBindTexture(GL_TEXTURE_2D, texture_normal->getID());
					}
				}
			}

			void OpenglRendererCommands::unbindMaterial() const
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
	}
}