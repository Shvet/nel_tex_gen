/** \file op_deform.h
 * Deformation filter interface
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

#ifndef NL_OP_DEFORM
#define NL_OP_DEFORM

#include "tex_gen_op.h"
#include "op_color.h"


namespace NLTEXGEN
{

/**
 * Deformation filter interface
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class COpDeform : public IOpColor
{
public:

	enum TInput
	{
		LastInput,
	};

	enum TParameter
	{
		TileU = IOpColor::LastParameter,	// Tile on / off
		TileV,								// Tile on / off
		Filter,								// Filter on / off
		LastParameter,
	};

	// Ctor. Init inputs and parameters
	COpDeform();

	// Streamable
	// 2NLMISC_DECLARE_CLASS(COpDeform);

	// Clone operator
	// NL_DECLARE_COPY_OPERATOR(COpDeform, ITexGenOperator);

protected:

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_DEFORM

/* End of op_deform.h */
