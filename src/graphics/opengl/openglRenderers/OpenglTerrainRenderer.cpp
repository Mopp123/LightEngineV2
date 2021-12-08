
#include <glew.h>
#include "OpenglTerrainRenderer.h"
#include "entities\Camera.h"
#include "core\Application.h"
#include "graphics\opengl\openglGraphicsUtils\OpenglGraphicsUtils.h"
#include "core\Debug.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			OpenglTerrainRenderer::OpenglTerrainRenderer()
			{
				_shader = (OpenglShader*)Shader::get_default_terrain_shader();
			}

			OpenglTerrainRenderer::~OpenglTerrainRenderer() 
			{}

			void OpenglTerrainRenderer::submit(const TerrainRenderable* renderable)
			{
				_renderQue.push_back(renderable);
			}

			void OpenglTerrainRenderer::flush() 
			{
				if (_renderQue.empty())
					return;

				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);


				RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				const Scene* currentScene = Application::get_scene_manager()->getCurrentScene();
				const Camera* camera = currentScene->getActiveCamera();
				const DirectionalLight* directionalLight = currentScene->getDirectionalLight();

				_shader->bind();

				_shader->setUniform_Mat4("projectionMatrix", camera->getProjectionMatrix());
				_shader->setUniform_Mat4("viewMatrix", camera->getViewMatrix());

				_shader->setUniform_Float3("directionalLight_direction", directionalLight->getDirection());
				_shader->setUniform_Float3("directionalLight.ambient", mml::Vec3(0.1f, 0.1f, 0.1f));
				_shader->setUniform_Float3("directionalLight.color", directionalLight->getColor());

				// for texture blending (doesn't necessarely need to be specified every single frame.. this is dumb..)
				_shader->setUniform_Int("texture_blendmap",			0);
				_shader->setUniform_Int("texture_channel_black",	1);
				_shader->setUniform_Int("texture_channel_red",		2);
				_shader->setUniform_Int("texture_channel_green",	3);
				_shader->setUniform_Int("texture_channel_blue",		4);


				for (const TerrainRenderable* terrain : _renderQue)
				{
					const OpenglMesh* mesh = (OpenglMesh*)terrain->getMesh();
					const Transform& transform = terrain->getEntity()->getTransform();
					_shader->setUniform_Mat4("transformationMatrix", transform.getTransformationMatrix());

					rendererCommands->bindMesh(mesh);
					bindTextures(mesh->getMaterial());
					
					glCall(glDrawElements(GL_TRIANGLES, mesh->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr));
					
					// unbind textures
					glActiveTexture(GL_TEXTURE0);
					glBindTexture(GL_TEXTURE_2D, 0);
				
					rendererCommands->unbindMesh(mesh);
				}

				_shader->unbind();

				_renderQue.clear();
			}


			void OpenglTerrainRenderer::bindTextures(const Material* material) const
			{
				// blendmap
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, material->getTexture(0)->getID());

				// black channel
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, material->getTexture(1)->getID());

				// red
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, material->getTexture(2)->getID());

				// green
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, material->getTexture(3)->getID());

				// blue
				glActiveTexture(GL_TEXTURE4);
				glBindTexture(GL_TEXTURE_2D, material->getTexture(4)->getID());
			}
		}
	}
}