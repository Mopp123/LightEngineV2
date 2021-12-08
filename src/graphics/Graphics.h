#pragma once


#include "Renderer.h"
#include "renderers\GUIRenderer.h"
#include "renderers\FontRenderer.h"
#include "renderers\MeshRenderer.h"
#include "renderers\SkinnedMeshRenderer.h"
#include "renderers\TerrainRenderer.h"

#include "RendererCommands.h"
#include "core\scene\Scene.h"

namespace lightEngine
{
	namespace graphics
	{
		enum class GraphicsAPI
		{
			None = 0, OpenGL, DirectX
		};

		class Graphics
		{
		private:

			static GraphicsAPI s_graphicsAPI;
			
			static GUIRenderer* s_guiRenderer;
			static FontRenderer* s_fontRenderer;
			static MeshRenderer* s_meshRenderer;
			static SkinnedMeshRenderer* s_skinnedMeshRenderer;
			static TerrainRenderer* s_terrainRenderer;

			static RendererCommands* s_rendererCommand;


		public:

			Graphics(GraphicsAPI graphicsApi);
			~Graphics();

			void submit(const gui::GUIImage* guiImg) const;
			void submit(const gui::Text* text) const;
			void submit(const Renderable3D* renderable3D) const;
			void submit(const SkinnedRenderable3D* skinnedRenderable3D) const;
			void submit(const TerrainRenderable* terrain) const;

			virtual void render(Scene* scene);

			static GraphicsAPI get_graphics_API();
			static RendererCommands* get_renderer_commands();
		};
	}
}