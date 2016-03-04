/** \file op_font.cpp
 * Font operator
 *
 * $Id: $
 */

/* Copyright, 2003 DIGITAL MURDER.
 *
 * This file is part of DIGITAL MURDER NEL.
 * DIGITAL MURDER NEL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.

 * DIGITAL MURDER NEL is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with DIGITAL MURDER NEL; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 */

#include "stdtex_gen.h"
#include "op_font.h"
#include "transform_float.h"

#include "nel/misc/path.h"

#include <freetype/freetype.h>

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************
const float SnapPixel[9]=
{
	0,
	0.25f,
	0.5f,
	0.75f,
	1.f,
	-0.25f,
	-0.5f,
	-0.75f,
	-1.f,
};

const char *FontFileName[]=
{
	"arial.ttf",
	"ariblk.ttf",
	"arialbd.ttf",
	"arialbi.ttf",
	"ariali.ttf",
	"comic.ttf",
	"comicdb.ttf",
	"cour.ttf",
	"courdb.ttf",
	"courbi.ttf",
	"couri.ttf",
	"georgia.ttf",
	"georgiab.ttf",
	"georgiaz.ttf",
	"georgiai.ttf",
	"impact.ttf",
	"lucon.ttf",
	"micross.ttf",
	"symbol.ttf",
	"tahoma.ttf",
	"tahomabd.ttf",
	"times.ttf",
	"timesbd.ttf",
	"timesbi.ttf",
	"timesi.ttf",
	"verdana.ttf",
	"verdanab.ttf",
	"verdanaz.ttf",
	"verdanai.ttf",
	"wingdings.ttf",
};

const char *FontName[]=
{
	"Arial",
	"Arial Black",
	"Arial Bold",
	"Arial Bold Italic",
	"Arial Italic",
	"Comic",
	"Comic Bold",
	"Courier",
	"Courier Bold",
	"Courier Bold Italic",
	"Courier",
	"Georgia",
	"Georgia Bold",
	"Georgia Bold Italic",
	"Georgia Italic",
	"Impact",
	"Lucon",
	"Microsoft Sans Sherif",
	"Symbol",
	"Tahoma",
	"Tahoma Bold",
	"Times New Roman",
	"Times New Roman Bold",
	"Times New Roman Bold Italic",
	"Times New Roman Italic",
	"Verdana",
	"Verdana Bold",
	"Verdana Bold Italic",
	"Verdana Italic",
	"Wingdings",
};

#define NLTEXGEN_FONT_COUNT (sizeof(FontName)/sizeof(const char*))

COpFont::COpFont()
{
	CFloatValueSetup PixelPositiveSetup (CFloatValueSetup::Linear, "times", 0, 1, 0.001f, true, true, SnapPixel, 9, 0.0001f);
	_Parameters.push_back (CParameter("text", "text", "", true));
	_Parameters.push_back (CParameter("font", "text", FontName, NLTEXGEN_FONT_COUNT));
	_Parameters.push_back (CParameter("height", "text", PixelPositiveSetup));
	_Parameters.push_back (CParameter("width factor", "text", ScaleSetup));
	_Parameters.push_back (CParameter("letter space", "text", PixelSetup));
	_Parameters.push_back (CParameter("line space", "text", ScaleSetup));
	_Parameters.push_back (CParameter(CParameter::TypeColor, "color", "text"));
	_Parameters.push_back (CParameter("tile x", "text", true));
	_Parameters.push_back (CParameter("tile y", "text", true));
	_Parameters.push_back (CParameter("x", "text", PixelSetup));
	_Parameters.push_back (CParameter("y", "text", PixelSetup));

	_Parameters[Height].Value = 0.1f;
	_Parameters[WidthFactor].Value = 1;
	_Parameters[LetterSpace].Value = 0;
	_Parameters[LineSpace].Value = 1;
	NLTEXGEN::copy(_Parameters[Foreground].Color, ColorWhite);
	_Parameters[PositionX].Value = 0.5f;
	_Parameters[PositionY].Value = 0.5f;
}

// ***************************************************************************

TChannel COpFont::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	FT_Error error;
	FT_Library	library;

	// Parameters

	// todo set parameters
	string &text = _Parameters[Text].StringValue;
	string fontFileName;
	if (_Parameters[Font].Integer < NLTEXGEN_FONT_COUNT)
	{
#ifdef NL_OS_WINDOWS
		char buffer[512];
		GetWindowsDirectory (buffer, 512);
		fontFileName = buffer + string ("\\fonts\\");
#endif NL_OS_WINDOWS
		fontFileName += FontFileName[_Parameters[Font].Integer];
	}
	uint32 charWidth = (uint32)floor(0.5f+(float)output.getHeight() * _Parameters[Height].Value * _Parameters[WidthFactor].Value);
	uint32 charHeight = (uint32)floor(0.5f+(float)output.getHeight() * _Parameters[Height].Value);
	float *foreground = _Parameters[Foreground].Color;
	bool tileX = _Parameters[TileU].Integer != 0;
	bool tileY = _Parameters[TileV].Integer != 0;
	sint dstX = (sint)floor(0.5f+(float)output.getWidth() * _Parameters[PositionX].Value);
	sint dstY = (sint)floor(0.5f+(float)output.getHeight() * _Parameters[PositionY].Value);
	sint dstXBackup = dstX;

	// Fill the bitmap
	foreground[3] = 0;
	replaceConst (output.getPixels(), foreground, 4*output.size(), ChannelAll);

	CFloatBitmap tempBitmap;

	if ((charWidth != 0) && (charHeight != 0))
	{
		error = FT_Init_FreeType (&library);
		if (!error)
		{
			FT_Face face;
			error = FT_New_Face (library, fontFileName.c_str(), 0, &face);
			if (!error)
			{
				string fontEx;
				
				// try to see if the ex filename exists based on the fontExFileName
				fontEx = CPath::lookup(CFile::getFilenameWithoutExtension (fontFileName)+".afm", false, false);

				if (fontEx != "")
				{
					FT_Attach_File (face, fontEx.c_str ());
				}

				error = FT_Select_Charmap (face, ft_encoding_unicode);
				if (!error)
				{
					error = FT_Set_Pixel_Sizes (face, charWidth, charHeight);
					if (!error)
					{
						uint i;
						for (i=0; i<text.size(); i++)
						{
							// Current char..
							

							// handle return
							if (text[i]=='\n')
							{
								dstX = dstXBackup;
								dstY += (sint)floor(0.5f+_Parameters[LineSpace].Value*_Parameters[Height].Value*(float)output.getWidth());
							}
							else
							{							
								if (text[i]>=32)
								{
									// retrieve glyph index from character code
									FT_UInt glyph_index = FT_Get_Char_Index (face, text[i]);

									// load glyph image into the slot (erase previous one)
									error = FT_Load_Glyph (face, glyph_index, FT_LOAD_DEFAULT);
									if (!error)
									{
										// convert to an anti-aliased bitmap
										error = FT_Render_Glyph (face->glyph, ft_render_mode_normal);
										if (!error)
										{
											uint width = face->glyph->bitmap.width;
											uint height = face->glyph->bitmap.rows;
											uint pitch = face->glyph->bitmap.pitch;
											tempBitmap.resize (width, height);

											int left = face->glyph->bitmap_left;
											int top = face->glyph->bitmap_top;

											int advx = face->glyph->advance.x;

											// Kerning
											int kernx = 0;
											if (i < (text.size()-1) && FT_HAS_KERNING(face))
											{
												FT_Vector  kerning;
												FT_Error error = FT_Get_Kerning (face, text[i], text[i+1], ft_kerning_default, &kerning);
												if (!error)
													kernx = kerning.x;
											}

											int glyphIndex = glyph_index;

											// Build the bitmap
											uint8 *pixels = (uint8*)face->glyph->bitmap.buffer;
											uint j;
											const uint count = tempBitmap.size();
											float *dst = tempBitmap.getPixels();
											for (j=0; j<count; j++, dst+=4)
											{
												dst[3] = (float)(pixels[j]) * (1.f/255.f);
											}

											// Copy the bitmap
											CLighten copy;
											copyPixels (output.getPixels(), output.getWidth(), output.getHeight(), dstX+left, dstY-top, 
												tempBitmap.getPixels(), tempBitmap.getWidth(), tempBitmap.getHeight(), tileX, tileY, copy);

											dstX += (sint)floor(0.5f+(float)output.getHeight() * _Parameters[Height].Value 
												* _Parameters[WidthFactor].Value * _Parameters[LetterSpace].Value) + (advx>>6)/* - kernx*/;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	
	
	return ChannelRGB;
}

// ***************************************************************************

/* End of op_font.cpp */
