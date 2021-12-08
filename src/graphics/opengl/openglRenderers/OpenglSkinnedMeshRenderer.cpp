
#include <glew.h>
#include "OpenglSkinnedMeshRenderer.h"
#include "graphics\opengl\openglGraphicsResources\OpenglMesh.h"
#include "graphics\graphicsResources\Material.h"
#include "animations\skeletalAnimation\Skeleton.h"
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
			OpenglSkinnedMeshRenderer::OpenglSkinnedMeshRenderer()
			{
			}

			OpenglSkinnedMeshRenderer::~OpenglSkinnedMeshRenderer()
			{}

			void OpenglSkinnedMeshRenderer::submit(const SkinnedRenderable3D* renderable)
			{
				submitRenderableData(renderable);
			}

			// JUST TEMP
#define MESH_RENDERER__INSTANCED_BUFFER_INDEX 1

			void OpenglSkinnedMeshRenderer::flush()
			{
				if (_renderQue.empty())
					return;

				glEnable(GL_DEPTH_TEST);

				RendererCommands* rendererCommands = Graphics::get_renderer_commands();
				const Scene* currentScene = Application::get_scene_manager()->getCurrentScene();
				const Camera* sceneCamera = currentScene->getActiveCamera();

				// All lighting uniforms..
				const DirectionalLight* directionalLight = currentScene->getDirectionalLight();
				
				for (const SkinnedRenderable3D* renderable : _renderQue)
				{
					const OpenglMesh* glMesh = (const OpenglMesh*)renderable->getMesh();
					Material* batchMaterial = glMesh->getMaterial();
					
					batchMaterial->setShader(Shader::get_default_skinned_mesh_shader());

					OpenglShader* batchShader = (OpenglShader*)batchMaterial->getShader();
					if (batchShader == nullptr)
						continue;

					batchShader->bind();
					
					// We always do this, no matter the material properties of the mesh!
					batchShader->setUniform_Mat4("projectionMatrix", sceneCamera->getProjectionMatrix());
					batchShader->setUniform_Mat4("viewMatrix", sceneCamera->getViewMatrix());
					batchShader->setUniform_Mat4("transformationMatrix", renderable->getEntity()->getTransform().getTransformationMatrix());
					
					// Bone matrices..
					const int maxBones = 50;
					std::vector<mml::Mat4> boneMatrices = renderable->getSkeleton()->getAllBoneMatrices();

					for (int i = 0; i < maxBones; i++)
					{
						if(i < boneMatrices.size())
							batchShader->setUniform_Mat4("boneMatrices[" + std::to_string(i) + "]", boneMatrices[i]);
						else
							batchShader->setUniform_Mat4("boneMatrices[" + std::to_string(i) + "]", mml::Mat4(1.0f));
					}

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

					glCall(glDrawElements(
						convert_prim_draw_type_to_gl(glMesh->getDrawType()),
						glMesh->getIndexBuffer()->getCount(),
						GL_UNSIGNED_INT,
						nullptr
					));

					rendererCommands->unbindMesh(glMesh);
					rendererCommands->unbindMaterial();

					batchShader->unbind();
				}

				_renderQue.clear();
			}
		}
	}
}