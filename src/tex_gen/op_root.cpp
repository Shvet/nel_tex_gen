/** \file op_root.cpp
 * Color operator
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
#include "op_root.h"
#include "transform_float.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

// ***************************************************************************

COpRoot::COpRoot()
{
	// Two inputs
	_Inputs.push_back(CInput("RGB"));
	_Inputs.push_back(CInput("Alpha"));
}

// ***************************************************************************

TChannel COpRoot::evalInternal (CFloatBitmap &output, const CRenderParameter &renderParameters)
{
	return evalSubOp (output, 0, renderParameters);
}

// ***************************************************************************

TChannel COpRoot::evalAlpha (class CFloatBitmap &dest, const CRenderParameter &renderParameters)
{
	return evalSubOp (dest, 1, renderParameters);
}

// ***************************************************************************

/* End of op_root.cpp */
