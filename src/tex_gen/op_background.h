/** \file op_background.h
 * Background operator
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

#ifndef NL_OP_BACKGROUND
#define NL_OP_BACKGROUND

#include "tex_gen/tex_gen_op.h"


namespace NLTEXGEN
{

/**
 * Background operator
 * Can specify bitmap size.
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpBackground : public ITexGenOperator
{
public:

	enum TParameter
	{
		Width = 0,		// The blend mode
		Height,			// The blend alpha
		Color,			// Normalize flag
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpBackground();

	// \from ITexGenOperator
	virtual TChannel evalInternal (class CFloatBitmap &output, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpBackground, Bg);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpBackground, ITexGenOperator);
};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_BACKGROUND

/* End of op_background.h */
