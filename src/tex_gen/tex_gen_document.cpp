/** \file tex_gen_document.cpp
 * Texture generator document
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
#include "tex_gen_op.h"
#include "tex_gen_document.h"

using namespace std;
using namespace NLMISC;
using namespace NLTEXGEN;

namespace NLTEXGEN
{

// ***************************************************************************

CTexGenDocument::~CTexGenDocument()
{
	uint i;
	for (i=0; i<Operators.size(); i++)
		delete Operators[i];
}

// ***************************************************************************

void CTexGenDocument::serial (NLMISC::IStream &s)
{
	// 4 octets for the header
	s.serialCheck((uint32)'GXTN');

	// File header
	FileVersion = s.serialVersion(2);

	// Serial the operator container
	s.serialContPolyPtr (Operators);
}

// ***************************************************************************

} // NLTEXGEN

/* End of bitmap_f.cpp */
