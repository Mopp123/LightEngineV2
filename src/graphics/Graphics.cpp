
#include <glew.h>
#include "core\Application.h"
#include "Graphics.h"
#include "core\Debug.h"


namespace lightEngine
{
	namespace graphics
	{
		GraphicsAPI Graphics::s_graphicsAPI = GraphicsAPI::OpenGL;

		
		// GUI renderers declarations..
		GUIRenderer*			Graphics::s_guiRenderer = nullptr;
		FontRenderer*			Graphics::s_fontRenderer = nullptr;
		MeshRenderer*			Graphics::s_meshRenderer = nullptr;
		SkinnedMeshRenderer*	Graphics::s_skinnedMeshRenderer = nullptr;
		TerrainRenderer*		Graphics::s_terrainRenderer = nullptr;

		RendererCommands* Graphics::s_rendererCommand = nullptr;

		Graphics::Graphics(GraphicsAPI graphicsApi)
		{
			s_graphicsAPI = graphicsApi;

			// Check current graphics API type
			if (s_graphicsAPI == GraphicsAPI::None ||
				s_graphicsAPI == GraphicsAPI::DirectX)
			{
				Debug::log("ERROR: Graphics api not supported yet!");
				Application::exit();
				return;
			}

			// Setup all graphics API stuff depending on which API we are currently using!
			// Shaders..
			Shader::create_default_shaders();

			// Renderers..
			s_guiRenderer =			GUIRenderer::create();
			s_fontRenderer =		FontRenderer::create();
			s_meshRenderer =		MeshRenderer::create();
			s_skinnedMeshRenderer = SkinnedMeshRenderer::create();
			s_terrainRenderer =		TerrainRenderer::create();

			s_rendererCommand =	RendererCommands::create();
			s_rendererCommand->setClearColor(mml::Vec4(0, 0, 0, 1));
		}

		Graphics::~Graphics()
		{}

		
		void Graphics::submit(const gui::GUIImage* guiImg) const
		{
			s_guiRenderer->submit(guiImg);
		}
		void Graphics::submit(const gui::Text* text) const
		{
			s_fontRenderer->submit(text);
		}
		void Graphics::submit(const Renderable3D* renderable) const
		{
			s_meshRenderer->submit(renderable);
		}
		void Graphics::submit(const SkinnedRenderable3D* skinnedRenderable3D) const
		{
			s_skinnedMeshRenderer->submit(skinnedRenderable3D);
		}
		void Graphics::submit(const TerrainRenderable* terrain) const
		{
			s_terrainRenderer->submit(terrain);
		}



		void Graphics::render(Scene* scene)
		{
			s_rendererCommand->clear();
			s_rendererCommand->clearColor();

			s_guiRenderer->flush();
			s_fontRenderer->flush();
			s_meshRenderer->flush();
			s_skinnedMeshRenderer->flush();
			s_terrainRenderer->flush();
		}


		GraphicsAPI Graphics::get_graphics_API()
		{
			return s_graphicsAPI;
		}


		RendererCommands* Graphics::get_renderer_commands()
		{
			return s_rendererCommand;
		}
	}
}