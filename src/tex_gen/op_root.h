/** \file op_root.h
 * The root operator
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

#ifndef NL_OP_ROOT_COLOR
#define NL_OP_ROOT_COLOR

#include "tex_gen_op.h"
#include "tex_gen/tex_gen_op.h"


namespace NLTEXGEN
{

/**
 * Root operator.
 * One per document
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpRoot : public ITexGenOperator
{
public:

	// Ctor. Init inputs and parameters
	COpRoot();

	/**
	  * Eval the operator alpha in the destination bitmap. Returns the channels that have been modified.
	  * If renderParameters.Cash is true, the result will be saved by the operator.
	  */
	TChannel	evalAlpha (class CFloatBitmap &dest, const CRenderParameter &renderParameters);

	// \from IOpColor
	virtual TChannel evalInternal (CFloatBitmap &output, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpRoot, Rt);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpRoot, ITexGenOperator);

protected:

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_ROOT_COLOR

/* End of op_root.h */
