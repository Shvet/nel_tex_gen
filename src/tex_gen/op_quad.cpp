/** \file op_quad.cpp
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
#include "op_quad.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpQuad::COpQuad()
{
	_Parameters.push_back (CParameter(CParameter::TypeColor, "outside color", "quad"));
	_Parameters.push_back (CParameter(CParameter::TypeColor, "inside color", "quad"));
	_Parameters.push_back (CParameter("width factor", "quad", PixelSetup));
	_Parameters.push_back (CParameter("height factor", "quad", PixelSetup));
	_Parameters.push_back (CParameter("x", "quad", PixelSetup));
	_Parameters.push_back (CParameter("y", "quad", PixelSetup));
	_Parameters.push_back (CParameter("filled", "quad", true));
	_Parameters.push_back (CParameter("border size", "quad", ScaleSetup));
	_Parameters.push_back (CParameter("tile x", "quad", true));
	_Parameters.push_back (CParameter("tile y", "quad", true));
	NLTEXGEN::copy (_Parameters[OutsideColor].Color, ColorWhite);
	NLTEXGEN::copy (_Parameters[InsideColor].Color, ColorBlack);
	_Parameters[Width].Value = 0.5f;
	_Parameters[Height].Value = 0.5f;
	_Parameters[PositionX].Value = 0.25f;
	_Parameters[PositionY].Value = 0.25f;
	_Parameters[BorderSize].Value = 0.02f;	
}

// ***************************************************************************

TChannel COpQuad::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	const uint width = output.getWidth();
	const uint height = output.getHeight();
	float *outside = _Parameters[OutsideColor].Color;
	float *inside = _Parameters[InsideColor].Color;
	const sint dstX = (sint)floor(0.5f+(float)width * _Parameters[PositionX].Value);
	const sint dstY = (sint)floor(0.5f+(float)height * _Parameters[PositionY].Value);
	const sint quadWidth = (sint)floor(0.5f+(float)width * _Parameters[Width].Value);
	const sint quadHeight = (sint)floor(0.5f+(float)height * _Parameters[Height].Value);
	const sint border = (sint)floor(0.5f+(float)width * _Parameters[BorderSize].Value);

	// Fill the bitmap
	outside[3] = 0;
	replaceConst (output.getPixels(), outside, 4*output.size(), ChannelAll);

	outside[3] = 1;
	inside[3] = 1;

	drawQuad (output.getPixels(), dstX, dstY, width, height, quadWidth, border, outside, _Parameters[TileU].Integer!=0, 
		_Parameters[TileV].Integer!=0);
	drawQuad (output.getPixels(), dstX, dstY+quadHeight-border, width, height, quadWidth, border, outside, _Parameters[TileU].Integer!=0, 
		_Parameters[TileV].Integer!=0);
	drawQuad (output.getPixels(), dstX, dstY+border, width, height, border, quadHeight - 2*border, outside, _Parameters[TileU].Integer!=0, 
		_Parameters[TileV].Integer!=0);
	drawQuad (output.getPixels(), dstX+quadWidth-border, dstY+border, width, height, border, quadHeight - 2*border, outside, _Parameters[TileU].Integer!=0, 
		_Parameters[TileV].Integer!=0);

	if (_Parameters[Filled].Integer != 0)
	{
		drawQuad (output.getPixels(), dstX+border, dstY+border, width, height, quadWidth-2*border, quadHeight-2*border, inside, _Parameters[TileU].Integer!=0, 
			_Parameters[TileV].Integer!=0);
	}

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_quad.cpp */
