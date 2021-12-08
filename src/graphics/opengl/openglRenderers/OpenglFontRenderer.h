#pragma once

#include "graphics\renderers\FontRenderer.h"
#include "graphics\opengl\openglShaders\GUIShaders\OpenglFontShader.h"

namespace lightEngine
{
	namespace graphics
	{
		namespace opengl
		{
			class OpenglFontRenderer : public FontRenderer
			{
			private:

				OpenglFontShader* _shader = nullptr;

			public:

				OpenglFontRenderer();
				virtual ~OpenglFontRenderer();

				virtual void submit(const gui::Text* text) override;
				virtual void flush() override;
			};
		}
	}
}