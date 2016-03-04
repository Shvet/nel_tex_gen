/** \file tex_gen_document.h
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

#ifndef NL_TEX_GEN_DOCUMENT
#define NL_TEX_GEN_DOCUMENT

namespace NLMISC
{
class CBitmap;
}

namespace NLTEXGEN
{

class ITexGenOperator;

// Texture generator document
class CTexGenDocument
{
public:

	// Destroy the operators
	~CTexGenDocument();

	// Serial it
	void serial (NLMISC::IStream &s);

	// Operators
	std::vector<ITexGenOperator*>	Operators;
};

// ***************************************************************************

} // NLTEXGEN

#endif // NL_TEX_GEN_DOCUMENT

/* End of tex_gen_document.h */
