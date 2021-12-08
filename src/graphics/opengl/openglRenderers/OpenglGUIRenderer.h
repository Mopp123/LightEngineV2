#pragma once

#include "graphics\renderers\GUIRenderer.h"
#include "graphics\opengl\openglShaders\GUIShaders\OpenglGUIImageShader.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglGUIRenderer : public GUIRenderer
			{
			private:

				OpenglGUIImageShader* _shader = nullptr;

			public:

				OpenglGUIRenderer();
				~OpenglGUIRenderer();

				virtual void submit(const gui::GUIImage* guiImage) override;
				virtual void flush() override;

			};
		}
	}
}