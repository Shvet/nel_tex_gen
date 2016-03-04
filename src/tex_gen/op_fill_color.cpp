/** \file op_fill_color.cpp
 * Fill with a color operator
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
#include "op_fill_color.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpFillColor::COpFillColor()
{
	// Parameters
	_Parameters.push_back (CParameter(CParameter::TypeColor, "fill color", "fill"));
}

// ***************************************************************************

TChannel COpFillColor::modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	// Blend the input and output
	float *out = output.getPixels();

	// Set RGB
	replaceConstRGBA (out, _Parameters[Color].Color, output.size()*4);

	return ChannelRGB;
}

// ***************************************************************************

/* End of op_color.cpp */
