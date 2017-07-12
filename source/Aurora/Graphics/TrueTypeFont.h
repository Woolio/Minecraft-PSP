#ifndef TRUETYPEFONT_H_
#define TRUETYPEFONT_H_

#include <Aurora/Graphics/Image.h>

typedef struct
{
	unsigned short x0,y0,x1,y1; // coordinates of bbox in bitmap
	float xoff,yoff,xadvance;   
} stbtt_bakedchar2;

typedef struct
{
	float x0,y0,s0,t0; // top-left
	float x1,y1,s1,t1; // bottom-right
} stbtt_aligned_quad2;

namespace Aurora
{
	namespace Graphics
	{
		enum TextAlign
		{
			ALIGN_LEFT,
			ALIGN_CENTER,
			ALIGN_RIGHT,
		};		

		class TrueTypeFont
		{

		private:

			Image* image;

		public:

			bool fontLoaded;
			stbtt_bakedchar2 g_cdata[96]; // ASCII 32..126 is 95 glyphs

			TrueTypeFont();
			TrueTypeFont(const char *fontFileName);
			TrueTypeFont(const char *fontFileName,float size);

			~TrueTypeFont();

			bool loadFont(const char *fontFileName,float fontSize);		
			void getBakedQuad(stbtt_bakedchar2 *chardata, int pw, int ph, int char_index,float *xpos, float *ypos, stbtt_aligned_quad2 *q);
			float getTextLength(stbtt_bakedchar2 *chardata, const char* text);

			void DrawText(float x, float y, const char *text, int align, unsigned int col);
		};
	}
}

#endif