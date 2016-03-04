/** \file op_solid.cpp
 * Solid operator
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
#include "op_solid.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

IOpSolid::IOpSolid()
{
	// Parameters
	_Parameters.push_back (CParameter("width", "solid", NLTEXGEN_SIZE_MIN, NLTEXGEN_SIZE_MAX));
	_Parameters.push_back (CParameter("height", "solid", NLTEXGEN_SIZE_MIN, NLTEXGEN_SIZE_MAX));
	_Parameters[Width].Integer = 256;
	_Parameters[Height].Integer = 256;
}

// ***************************************************************************

TChannel IOpSolid::evalInternal (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	// Resize the output bitmap
	output.resize (_Parameters[Width].Integer, _Parameters[Height].Integer);
	
	// return the modified channels
	return fillSolid (output);
}

// ***************************************************************************

/* End of op_solid.cpp */
