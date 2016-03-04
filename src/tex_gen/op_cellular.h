/** \file op_cellular.h
 * Fill with cellular
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

#ifndef NL_OP_CELLUAR
#define NL_OP_CELLUAR

#include "tex_gen_op.h"
#include "tex_gen/op_layer.h"


namespace NLTEXGEN
{

/**
 * Cellular operator.
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpCellular : public IOpLayer
{
public:

	enum TInput
	{
		LastInput = IOpLayer::LastInput,			// The gradient color to use
	};

	enum TParameter
	{
		Points = IOpLayer::LastParameter,			// The gradient color to use
		Seed,
		Mode,
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpCellular();

	// \from IOpLayer
	virtual TChannel modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters);

	// Streamable
	NLMISC_DECLARE_CLASS2(COpCellular, Cr);

	// Clone operator
	NL_DECLARE_COPY_OPERATOR(COpCellular, ITexGenOperator);

protected:

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_CELLUAR

/* End of op_noise.h */
