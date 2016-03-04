/** \file op_layer.h
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

#ifndef NL_OP_LAYER
#define NL_OP_LAYER

#include "tex_gen/op_color.h"


namespace NLTEXGEN
{

/**
 * Simple color modificator operator.
 * Derived operator must use only one input and must return a bitmap of same size than the input.
 *
 * \author Cyril 'Hulud' Corvazier
 * \author DIGITAL MURDER
 * \date 2003
 */
class IOpLayer : public IOpColor
{
public:

	enum TInput
	{
		LastInput = IOpColor::LastInput,
	};

	enum TParameter
	{
		LastParameter = IOpColor::LastParameter,
	};

	// Ctor. Init inputs and parameters
	IOpLayer();

	// \from IOpLayer
	virtual TChannel modifyColor (CFloatBitmap &output, const CFloatBitmap &input, const CRenderParameter &renderParameters);

	/** 
	  * Interface to implement by the layer operator.
	  * This operator must change the size of output with the size of input.
	  * output and input can be the same object sometimes.
	  *
	  * This method must returns the modifed channels.
	  */
	virtual TChannel modifyLayer (CFloatBitmap &output, const CRenderParameter &renderParameters) = 0;

	// Get the layer cached bitmap
	const CFloatBitmap	&getCachedLayer () const;

private:
	
	// The cash bitmap
	CFloatBitmap				_LayerCache;

};

// ***************************************************************************

} // NLTEXGEN


#endif // NL_OP_LAYER

/* End of op_layer.h */
