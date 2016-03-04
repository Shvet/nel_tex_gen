/** \file op_layer.cpp
 * Layer operator
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
#include "op_layer.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

IOpLayer::IOpLayer()
{
}

// ***************************************************************************

TChannel IOpLayer::modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters)
{
	// Evaluate the layer
	output.resize (input.getWidth(), input.getHeight());
	TChannel channels = modifyLayer (output, renderParameters);

	if (renderParameters.Cache)
	{
		// Copy the output into the cache
		_LayerCache = output;
	}

	// return the modified channels
	return channels;
}

// ***************************************************************************

const CFloatBitmap	&IOpLayer::getCachedLayer () const
{
	return _LayerCache;
}

// ***************************************************************************

/* End of op_layer.cpp */
