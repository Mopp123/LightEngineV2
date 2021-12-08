
#include <glew.h>
#include "OpenglMeshRenderer.h"
#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"
#include "graphics\graphicsResources\Material.h"
#include "graphics\opengl\openglShaders\OpenglShader.h"
#include "core\Application.h"
#include "core\Debug.h"
#include <string>
#include "graphics\opengl\openglGraphicsUtils\OpenglGraphicsUtils.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			OpenglMeshRenderer::OpenglMeshRenderer()
			{
			}

			OpenglMeshRenderer::~OpenglMeshRenderer()
			{}

			void OpenglMeshRenderer::submit(const Renderable3D* renderable)
			{
				submitRenderableData(renderable);
			}

			// JUST TEMP
#define MESH_RENDERER__INSTANCED_BUFFER_INDEX 1

			void OpenglMeshRenderer::flush()
			{
				if (_batches.empty())
					return;

				glEnable(GL_DEPTH_TEST);

				RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				const Scene* currentScene = Application::get_scene_manager()->getCurrentScene();
				const Camera* sceneCamera = currentScene->getActiveCamera();

				// All lighting uniforms..
				const DirectionalLight* directionalLight = currentScene->getDirectionalLight();

				for (std::pair<const Mesh*, std::vector<float>>& batch : _batches)
				{
					const OpenglMesh* glMesh = (const OpenglMesh*)batch.first;
					Material* batchMaterial = glMesh->getMaterial();
					
					OpenglShader* batchShader = (OpenglShader*)batchMaterial->getShader();
					if (batchShader == nullptr)
						continue;

					batchShader->bind();

					// We always do this, no matter the material properties of the mesh!
					batchShader->setUniform_Mat4("projectionMatrix", sceneCamera->getProjectionMatrix());
					batchShader->setUniform_Mat4("viewMatrix", sceneCamera->getViewMatrix());
					batchShader->setUniform_Float3("viewPos", sceneCamera->getTransform().getGlobalPosition());

					batchShader->setUniform_Float3("directionalLight_direction", directionalLight->getDirection());
					batchShader->setUniform_Float3("directionalLight.ambient", mml::Vec3(0.1f, 0.1f, 0.1f));
					batchShader->setUniform_Float3("directionalLight.color", directionalLight->getColor());

					// WE always assume that a 3d mesh has at least diffuse texture..
					batchShader->setUniform_Int("diffuseTexture", 0);

					// JUST TESTING
					if (batchMaterial->getTexture_normal())
					{
						batchShader->setUniform_Float("material.specularStrength", batchMaterial->getSpecular_strength());
						batchShader->setUniform_Float("material.specularShininess", batchMaterial->getSpecular_shininess());
						batchShader->setUniform_Int("specularTexture", 1);
						batchShader->setUniform_Int("normalTexture", 2);
					}

					rendererCommands->bindMesh(glMesh);
					rendererCommands->bindMaterial(batchMaterial);

					std::vector<float>& instancedData = batch.second;
					// Update the instanced data buffer of this mesh..
					glMesh->getVertexBuffers()[MESH_RENDERER__INSTANCED_BUFFER_INDEX]->update(&instancedData[0], instancedData.size(), 0);

					// Count of the renderables in this batch
					const unsigned int renderableCount = instancedData.size() / MESH_RENDERER__INSTANCED_DATA_ENTRY_LENGTH;

					glCall(glDrawElementsInstanced(
						convert_prim_draw_type_to_gl(glMesh->getDrawType()), 
						glMesh->getIndexBuffer()->getCount(), 
						GL_UNSIGNED_INT, 
						nullptr, 
						renderableCount
					));
					
					batchShader->unbind();

					rendererCommands->unbindMesh(glMesh);
					rendererCommands->unbindMaterial();
				}

				//_shader->unbind();

				_batches.clear();
			}
		}
	}
}